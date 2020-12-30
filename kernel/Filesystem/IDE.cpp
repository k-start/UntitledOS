#include <kernel/Filesystem/IDE.h>
#include <kernel/ports.h>
#include <kernel/kstdio.h>
#include <kernel/PIT.h>

#define BUS_ADDR 0x1F0

IDE *IDE::the;

IDE::IDE() {
    the = this;

    init(BUS_ADDR);
}

void IDE::ataIOwait(uint16_t bus) {
    inb(bus + REG_ALTSTATUS);
    inb(bus + REG_ALTSTATUS);
    inb(bus + REG_ALTSTATUS);
    inb(bus + REG_ALTSTATUS);
}

int IDE::ataWait(uint16_t bus, int advanced) {
    uint8_t status = 0;

    ataIOwait(bus);

    while((status = inb(bus + REG_STATUS)) & SR_BSY);

    if(advanced) {
        status = inb(bus + REG_STATUS);

        if(status & SR_ERR) {
            sout("[IDE] SR_ERR\n");
            return 1;
        }
        if(status & SR_DF) {
            sout("[IDE] SR_DF\n");
            return 1;
        }
        if(!(status & SR_DRQ)) {
            sout("[IDE] SR_DRQ\n");
            return 1;
        }
    }

    return 0;
}

void IDE::ataSelect(uint16_t bus) {
    outb(bus + REG_HDDEVSEL, 0xA0);
}

void IDE::ataWaitReady(uint16_t bus) {
    while (inb(bus + REG_STATUS) & SR_BSY);
}

void IDE::init(uint16_t bus) {
    sout("[IDE] Initializing IDE device on bus %d\n", bus);

    outb(bus + 1, 1);
    outb(bus + 0x306, 0);

    ataSelect(bus);
    ataIOwait(bus);

    outb(bus + REG_COMMAND, CMD_IDENTIFY);
    ataIOwait(bus);

    int status = inb(bus + REG_COMMAND);
    sout("[IDE] Status = 0x%x\n", status);

    ataWaitReady(bus);

    ATAIdentify device;
    uint16_t* buf = (uint16_t*)&device;

    for(int i = 0; i < 256; ++i) {
        buf[i] = ins(bus);
    }

    uint8_t* ptr = (uint8_t*)&device.model;
    for(int i = 0; i < 39; i += 2) {
        uint8_t tmp = ptr[i + 1];
        ptr[i + 1] = ptr[i];
        ptr[i] = tmp;
    }
    ptr[40] = '\0';

    sout("[IDE] Device - %s\n", device.model);
    sout("[IDE] Sectors_48 = %d\n", (uint32_t)device.sectors_48);
    sout("[IDE] Sectors_28 = %d\n", device.sectors_28);
    sout("[IDE] Size = %dMB\n", ((device.sectors_28 / 2) / 1024));

    outb(bus + REG_CONTROL, 0x02);
}

void IDE::readSector(uint16_t bus, uint8_t slave, uint32_t lba, uint8_t* buf) {
    outb(bus + REG_CONTROL, 0x02);

    ataWaitReady(bus);

    outb(bus + REG_HDDEVSEL,  0xe0 | slave << 4 | (lba & 0x0f000000) >> 24);
    outb(bus + REG_FEATURES, 0x00);
	outb(bus + REG_SECCOUNT0, 1);
	outb(bus + REG_LBA0, (lba & 0x000000ff) >>  0);
	outb(bus + REG_LBA1, (lba & 0x0000ff00) >>  8);
	outb(bus + REG_LBA2, (lba & 0x00ff0000) >> 16);
	outb(bus + REG_COMMAND, CMD_READ_PIO);

    if (ataWait(bus, 1)) {
		sout("[IDE] Error during ATA read\n");
	}

    int size = 256;
	insm(bus, buf, size);
	ataWait(bus, 0);
}

uint8_t* IDE::readSector(uint32_t sector) {
    uint8_t* buf = new uint8_t[512];

    readSector(BUS_ADDR, 0, sector, buf);

    return buf;
}

void IDE::writeSector(uint16_t bus, uint8_t slave, uint32_t lba, uint8_t* buf) {
    outb(bus + REG_CONTROL, 0x02);

    ataWaitReady(bus);

    outb(bus + REG_HDDEVSEL, 0xE0 | slave << 4 | (lba & 0x0F000000) >> 24);
    ataWait(bus, 0);
    outb(bus + REG_FEATURES, 0x00);
    outb(bus + REG_SECCOUNT0, 0x01);
    outb(bus + REG_LBA0, (lba & 0x000000FF) >>  0);
	outb(bus + REG_LBA1, (lba & 0x0000FF00) >>  8);
	outb(bus + REG_LBA2, (lba & 0x00FF0000) >> 16);
	outb(bus + REG_COMMAND, CMD_WRITE_PIO);
	outb(bus, 0);

    int size = 256;

    outsm(bus, buf, size);
    outb(bus + 0x07, CMD_CACHE_FLUSH);
	ataWait(bus, 0);
}

void IDE::writeSector(uint32_t sector, uint8_t* buf) {
    writeSector(BUS_ADDR, 0, sector, buf);
}