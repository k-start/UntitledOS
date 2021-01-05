#ifndef FAT12_H
#define FAT12_H

#include <stdint.h>
#include <kernel/Filesystem/VFS.h>
#include <Y/String.h>

#define SECTOR_SIZE 512

class FAT12 {
    public:

        typedef struct _BOOTSECTOR {
            uint8_t  unused[3];
            uint8_t  OEM[8];
            uint16_t bytesPerSector;
            uint8_t  sectorsPerCluster;
            uint16_t reservedSectors;
            uint8_t  FATs;
            uint16_t directoryEntries;
            uint16_t sectorsCount;
            uint8_t  media;
            uint16_t sectorsPerFAT;
            uint16_t sectorsPerTrack;
            uint16_t headsCount;
            uint32_t hiddenSectorsCount;
            uint32_t largeSectorCount;

            uint8_t  driveNumber;
            uint8_t  NTFlags;
            uint8_t  signature;
            uint32_t volumeID;
            uint8_t  volumeLabel[11];
            uint8_t  systemIdentifier[8];
            uint8_t  bootCode[448];
            uint16_t bootablePartitionSignature;
        } __attribute__((packed)) BOOTSECTOR, *PBOOTSECTOR;

        typedef struct _MOUNT_INFO {
            uint32_t sectorsCount;
            uint32_t FATOffset;
            uint32_t rootEntriesCount;
            uint32_t rootOffset;
            uint32_t rootSize;
            uint32_t FATSize;
            uint32_t FATEntrySize;
        } MOUNT_INFO, *PMOUNT_INFO;

        typedef struct _DIRECTORY {
            uint8_t  filename[8];
            uint8_t  extension[3];
            uint8_t  attributes;
            uint8_t  windowsNTRESERVED;
            uint8_t  creationTimeMS;
            uint16_t creationTime;
            uint16_t creationDate;
            uint16_t lastAccessedDate;
            uint16_t firstClusterHi;
            uint16_t lastModifiedTime;
            uint16_t lastModifiedDate;
            uint16_t firstClusterLow;
            uint32_t size;
        } DIRECTORY, *PDIRECTORY;

        FAT12();

        FILE directory(String name);
        void read(PFILE file, uint8_t* buf, int len);
        void write(PFILE file, uint8_t* buf, int len);

        void close(PFILE file);
        FILE open(String name);
        FILE openSubDir(FILE kFile, String name);

        Vector<String> list();

        void writetest(String filename, uint8_t* buf, int len);

    private:

        Vector<int> findSectors(int sectorCount);
        void updateFAT(Vector<int> sectors);

        PBOOTSECTOR bootsector;
        MOUNT_INFO mountInfo;

        uint8_t FAT[SECTOR_SIZE*2];
};

#endif