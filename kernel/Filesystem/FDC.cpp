#include <kernel/Filesystem/FDC.h>
#include <kernel/kstdio.h>
#include <kernel/ports.h>
#include <kernel/PIT.h>

bool IRQtriggered = false;

const int DMA_BUFFER = 0x1000 + 0xC0000000;

FDC::FDC(u8 IRQNumber) : IRQHandler(IRQNumber) {
    initializeDma();
    reset();
    // driveData(13, 1, 0xf, true);
}

void FDC::handleIRQ() {
    sout("FLPY: IRQ\n");
    IRQtriggered = true;
}

// initializes DMA to use physical address 1k-64l
void FDC::initializeDma() {
    outb(0x0a, 0x06);	//mask dma channel 2
	outb(0xd8, 0xff);	//reset master flip-flop
	outb(0x04, 0);     //address=0x1000 
	outb(0x04, 0x10);
	outb(0xd8, 0xff);  //reset master flip-flop
	outb(0x05, 0xff);  //count to 0x23ff (number of bytes in a 3.5" floppy disk track)
	outb(0x05, 0x23);
	outb(0x80, 0);     //external page register = 0
	outb(0x0a, 0x02);  //unmask dma channel 2
}

// Prepares DMA for read
void FDC::dmaRead() {
    outb(0x0a, 0x06);  //mask dma channel 2
	outb(0x0b, 0x56);  //single transfer, address increment, autoinit, read, channel 2
	outb(0x0a, 0x02);  //unmask dma channel 2
}

// Prepares DMA for write
void FDC::dmaWrite() {
    outb(0x0a, 0x06);  //mask dma channel 2
	outb(0x0b, 0x5a);  //single transfer, address increment, autoinit, write, channel 2
	outb(0x0a, 0x02);  //unmask dma channel 2
}

void FDC::writeDOR(uint8_t value) {
    outb(IO_DOR, value);
}

uint8_t FDC::readStatus() {
    return inb(IO_MSR);
}

void FDC::sendCommand(uint8_t cmd) {
    for (int i = 0; i < 500; i++) {
        if (readStatus() & MSR_MASK_DATAREG) { // bitwise AND is correct
            outb(IO_FIFO, cmd);
        }
    }
}

uint8_t FDC::readData() {
    for (int i = 0; i < 500; i++) {
        if (readStatus() & MSR_MASK_DATAREG) { // bitwise AND is correct
            return inb(IO_FIFO);
        }
    }
}

void FDC::writeCCR(uint8_t value) {
    outb(IO_CTRL, value);
}

void FDC::waitforIRQ() {
    while(!IRQtriggered) { } ;
    IRQtriggered = false;
}

void FDC::readSector(uint8_t head, uint8_t track, uint8_t sector) {
    sout("FLPY: Read Sector imp START\n");
    uint32_t st0, cyl;

    IRQtriggered = false;

    dmaRead();

    sendCommand(  CMD_READ_SECT
                | CMD_EXT_MULTITRACK
                | CMD_EXT_SKIP
                | CMD_EXT_DENSITY);

    sendCommand(head << 2 | currentDrive);
    sendCommand(track);
    sendCommand(head);
    sendCommand(sector);
    sendCommand(SECTOR_DTL_512);

    sendCommand(
        ((sector + 1) >= SECTORS_PER_TRACK) ? SECTORS_PER_TRACK : sector + 1
    );

    sendCommand(GAP3_LENGTH_3_5);
    sendCommand(0xff);

    // waitforIRQ();
    while(!IRQtriggered);

    for(int i = 0; i < 7; i++) {
        readData();
    }

    checkInt(&st0, &cyl);
    sout("FLPY: Read Sector imp DONE\n");
}

void FDC::driveData(uint32_t stepr, uint32_t loadt, uint32_t unloadt, bool dma) {
    uint32_t data = 0;

    sendCommand(CMD_SPECIFY);

    data = ((stepr & 0xf) << 4) | (unloadt & 0xf);
    sendCommand(data);
    // sout("0x%x\n",data);

    data = (loadt << 1) | (dma ? 0:1);
	sendCommand(data);
    // sout("0x%x\n",data);
}

int FDC::calibrate(uint32_t drive) {
    uint32_t st0, cyl;

    if(drive >= 4) {
        return -2;
    }

    controlMotor(true);

    for(int i = 0; i < 10; i++) {
        IRQtriggered = false;

        sendCommand(CMD_CALIBRATE);
		sendCommand(drive);

        // waitforIRQ();
        while(!IRQtriggered);
        checkInt(&st0, &cyl);

        if(!cyl) {
            controlMotor(false);
			return 0;
        }
    }

    controlMotor(false);
    return -1;
}

void FDC::checkInt(uint32_t* st0, uint32_t* cyl) {
    sendCommand(CMD_CHECK_INT);

    *st0 = readData();
    *cyl = readData();
}

int FDC::seek(uint32_t cyl, uint32_t head) {
    sout("FLPY: Seek START\n");
    uint32_t st0, cyl0;

    if(currentDrive >= 4) {
        return -2;
    }

    controlMotor(true);

    for(int i = 0; i < 10; i++) {
        IRQtriggered = false;
        sendCommand(CMD_SEEK);
        sendCommand(head << 2 | currentDrive);
        sendCommand(cyl);

        // waitforIRQ();
        while(!IRQtriggered);
        checkInt(&st0, &cyl0);

        if(cyl0 == cyl) {
            controlMotor(false);
            sout("FLPY: Seek DONE\n");
            return 0;
        }
    }

    controlMotor(false);
    sout("FLPY: Seek DONE\n");

    return -1;
}

void FDC::disableController() {
    writeDOR(0);
}

void FDC::enableController() {
    writeDOR(DOR_MASK_RESET | DOR_MASK_DMA);
}

void FDC::reset() {
    sout("FLPY: Reset START\n");
    uint32_t st0, cyl;

    IRQtriggered = false;

    disableController();
    enableController();

    while(!IRQtriggered);
    // waitforIRQ();

    for(int i = 0; i < 4; i++) {
        checkInt(&st0, &cyl);
    }

    writeCCR(0);

    // driveData(3, 16, 240, true);
    calibrate(currentDrive);

    sout("FLPY: Reset DONE\n");
}

void FDC::controlMotor(bool b) {
    if(currentDrive > 3) {
        return;
    }

    uint32_t motor = 0;

    switch(currentDrive) {
        case 0:
            motor = DOR_MASK_DRIVE0_MOTOR;
            break;
        case 1:
            motor = DOR_MASK_DRIVE1_MOTOR;
            break;
        case 2:
            motor = DOR_MASK_DRIVE2_MOTOR;
            break;
        case 3:
            motor = DOR_MASK_DRIVE3_MOTOR;
            break;
    }

    if(b) {
        writeDOR(currentDrive | motor | DOR_MASK_RESET | DOR_MASK_DMA);
    } else {
        writeDOR(DOR_MASK_RESET);
    }
    
    // PIT::the->sleep(200);
}

uint8_t* FDC::readSector2(int sectorlba) {
    sout("FLPY: Read Sector lba START\n");
    if(currentDrive >= 4) {
        return 0;
    }

    int head = 0, track = 0, sector = 1;
    lbaToChs(sectorlba, &head, &track, &sector);

    controlMotor(true);

    if (seek(track, head) != 0) {
        return 0;
    }

    readSector(head, track, sector);
    controlMotor(false);

    sout("FLPY: Read Sector lba DONE\n");
    return (uint8_t*) DMA_BUFFER;
}

void FDC::lbaToChs(int lba,int *head,int *track,int *sector) {
   *head = ( lba % ( SECTORS_PER_TRACK * 2 ) ) / ( SECTORS_PER_TRACK );
   *track = lba / ( SECTORS_PER_TRACK * 2 );
   *sector = lba % SECTORS_PER_TRACK + 1;
}