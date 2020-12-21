#ifndef FDC_H
#define FDC_H

#include <kernel/IRQHandler.h>
#include <Types.h>
#include <stdint.h>
#include <stddef.h>

class FDC : public IRQHandler {

    public:
        enum IO {
            IO_DOR	= 0x3f2,
            IO_MSR	= 0x3f4,
            IO_FIFO =	0x3f5,	//data register
            IO_CTRL =	0x3f7
        };

        enum DOR_MASK {       
            DOR_MASK_DRIVE0 = 0,	    //00000000	= here for completeness sake
            DOR_MASK_DRIVE1 = 1,	    //00000001
            DOR_MASK_DRIVE2 = 2,	    //00000010
            DOR_MASK_DRIVE3 = 3,	    //00000011
            DOR_MASK_RESET = 4,	        //00000100
            DOR_MASK_DMA = 8,	        //00001000
            DOR_MASK_DRIVE0_MOTOR = 16,	//00010000
            DOR_MASK_DRIVE1_MOTOR = 32,	//00100000
            DOR_MASK_DRIVE2_MOTOR = 64,	//01000000
            DOR_MASK_DRIVE3_MOTOR = 128	//10000000
        };

        enum MSR_MASK {
            MSR_MASK_DRIVE1_POS_MODE = 1,	//00000001
            MSR_MASK_DRIVE2_POS_MODE = 2,	//00000010
            MSR_MASK_DRIVE3_POS_MODE = 4,	//00000100
            MSR_MASK_DRIVE4_POS_MODE = 8,	//00001000
            MSR_MASK_BUSY = 16,	            //00010000
            MSR_MASK_DMA = 32,  	        //00100000
            MSR_MASK_DATAIO = 64, 	        //01000000
            MSR_MASK_DATAREG = 128  	    //10000000
        };

        enum CMD {
            CMD_READ_TRACK 	= 2,
            CMD_SPECIFY = 3,
            CMD_CHECK_STAT = 4,
            CMD_WRITE_SECT = 5,
            CMD_READ_SECT = 6,
            CMD_CALIBRATE = 7,
            CMD_CHECK_INT = 8,
            CMD_WRITE_DEL_S = 9,
            CMD_READ_ID_S = 0xa,
            CMD_READ_DEL_S = 0xc,
            CMD_FORMAT_TRACK = 0xd,
            CMD_SEEK = 0xf
        };

        enum CMD_EXT {
            CMD_EXT_SKIP = 0x20,	    //00100000
            CMD_EXT_DENSITY = 0x40, 	//01000000
            CMD_EXT_MULTITRACK = 0x80	//10000000
        };

        enum GAP3_LENGTH {
	        GAP3_LENGTH_STD = 42,
	        GAP3_LENGTH_5_14 = 32,
            GAP3_LENGTH_3_5 = 27
        };

        enum SECTOR_DTL {
            SECTOR_DTL_128 = 0,
            SECTOR_DTL_256 = 1,
            SECTOR_DTL_512 = 2,
            SECTOR_DTL_1024 = 4
        };

        const uint8_t SECTORS_PER_TRACK = 18;

        FDC(u8 IRQNumber);

        virtual void handleIRQ() override;

        static FDC *the;

        void initializeDma();
        void dmaRead();
        void dmaWrite();

        void writeDOR(uint8_t value);
        uint8_t readStatus();
        void sendCommand(uint8_t cmd);
        uint8_t readData();
        void writeCCR(uint8_t value);

        void waitforIRQ();
        void readSector(uint8_t head, uint8_t track, uint8_t sector);
        void driveData(uint32_t stepr, uint32_t loadt, uint32_t unloadt, bool dma);
        int calibrate(uint32_t drive);
        void checkInt(uint32_t *st0, uint32_t *cyl);
        int seek(uint32_t cyl, uint32_t head);
        void disableController();
        void enableController();
        void reset();
        void controlMotor(bool b);

        uint8_t* readSector2(int sectorlba);
        void lbaToChs(int lba,int *head,int *track,int *sector);

    private:
        uint8_t currentDrive = 0;

};

#endif