#ifndef IDE_H
#define IDE_H

#include <Types.h>
#include <stdint.h>

class IDE {

    public:

        enum status {
            SR_BSY  = 0x80,
            SR_DRDY = 0x40,
            SR_DF   = 0x20,
            SR_DSC  = 0x10,
            SR_DRQ  = 0x08,
            SR_CORR = 0x04,
            SR_IDX  = 0x02,
            SR_ERR  = 0x01
        };

        enum errors {
            ER_BBK   = 0x80,
            ER_UNC   = 0x40,
            ER_MC    = 0x20,
            ER_IDNF  = 0x10,
            ER_MCR   = 0x08,
            ER_ABRT  = 0x04,
            ER_TK0NF = 0x02,
            ER_AMNF  = 0x01
        };

        enum commands {
            CMD_READ_PIO_EXT    = 0x24,
            CMD_READ_DMA        = 0xC8,
            CMD_READ_PIO        = 0x20,
            CMD_READ_DMA_EXT    = 0x25,
            CMD_WRITE_PIO       = 0x30,
            CMD_WRITE_PIO_EXT   = 0x34,
            CMD_WRITE_DMA       = 0xCA,
            CMD_WRITE_DMA_EXT   = 0x35,
            CMD_CACHE_FLUSH     = 0xE7,
            CMD_CACHE_FLUSH_EXT = 0xEA,
            CMD_PACKET          = 0xA0,
            CMD_IDENTIFY_PACKET = 0xA1,
            CMD_IDENTIFY        = 0xEC,

            ATAPI_CMD_READ      = 0xA8,
            ATAPI_CMD_EJECT     = 0x1B
        };

        enum identify {
            IDENT_DEVICETYPE   = 0,
            IDENT_CYLINDERS    = 2,
            IDENT_HEADS        = 6,
            IDENT_SECTORS      = 12,
            IDENT_SERIAL       = 20,
            IDENT_MODEL        = 54,
            IDENT_CAPABILITIES = 98,
            IDENT_FIELDVALID   = 106,
            IDENT_MAX_LBA      = 120,
            IDENT_COMMANDSETS  = 164,
            IDENT_MAX_LBA_EXT  = 200
        };

        enum reg {
            REG_DATA       = 0x00,
            REG_ERROR      = 0x01,
            REG_FEATURES   = 0x01,
            REG_SECCOUNT0  = 0x02,
            REG_LBA0       = 0x03,
            REG_LBA1       = 0x04,
            REG_LBA2       = 0x05,
            REG_HDDEVSEL   = 0x06,
            REG_COMMAND    = 0x07,
            REG_STATUS     = 0x07,
            REG_SECCOUNT1  = 0x08,
            REG_LBA3       = 0x09,
            REG_LBA4       = 0x0A,
            REG_LBA5       = 0x0B,
            REG_CONTROL    = 0x0C,
            REG_ALTSTATUS  = 0x0C,
            REG_DEVADDRESS = 0x0D
        };

        struct IDEChannelRegisters {
            uint16_t base;  // I/O Base.
            uint16_t ctrl;  // Control Base
            uint16_t bmide; // Bus Master IDE
            uint16_t nien;  // nien (No Interrupt);
        };

        struct IDEDevice {
            uint8_t  reserved;    // 0 (Empty) or 1 (This Drive really exists).
            uint8_t  channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
            uint8_t  drive;       // 0 (Master Drive) or 1 (Slave Drive).
            uint16_t type;        // 0: ATA, 1:ATAPI.
            uint16_t signature;   // Drive Signature
            uint16_t capabilities;// Features.
            uint32_t commandSets; // Command Sets Supported.
            uint32_t size;        // Size in Sectors.
            uint8_t  Model[41];   // Model in string.
        };

        struct ATAIdentify {
            uint16_t flags;
            uint16_t unused1[9];
            char     serial[20];
            uint16_t unused2[3];
            char     firmware[8];
            char     model[40];
            uint16_t sectors_per_int;
            uint16_t unused3;
            uint16_t capabilities[2];
            uint16_t unused4[2];
            uint16_t valid_ext_data;
            uint16_t unused5[5];
            uint16_t size_of_rw_mult;
            uint32_t sectors_28;
            uint16_t unused6[38];
            uint64_t sectors_48;
            uint16_t unused7[152];
        } __attribute__((packed));

        IDE();
        static IDE *the;

        void init(uint16_t bus);

        uint8_t* readSector(uint32_t sector);
        void writeSector(uint32_t sector, uint8_t* buf);

    private:
        void ataIOwait(uint16_t bus);
        int ataWait(uint16_t bus, int advanced);
        void ataSelect(uint16_t bus);
        void ataWaitReady(uint16_t bus);

        void readSector(uint16_t bus, uint8_t slave, uint32_t lba, uint8_t* buf);
        void writeSector(uint16_t bus, uint8_t slave, uint32_t lba, uint8_t* buf);

        // IDEChannelRegisters channels[2];
        // IDEDevice devices[4];
        // u8 ide_buf[2048] = {0};
        // u8 ide_irq_invoked = 0;
        // u8 atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

};

#endif