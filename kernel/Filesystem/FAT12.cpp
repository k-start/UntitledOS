#include <kernel/Filesystem/FAT12.h>
#include <kernel/kstdio.h>
#include <kernel/Filesystem/IDE.h>
#include <string.h>
#include <Y/Vector.h>
#include <kernel/RTC.h>

FAT12::FAT12() {
    bootsector = (PBOOTSECTOR)IDE::the->readSector(0);

    mountInfo.sectorsCount = bootsector->sectorsCount;
    mountInfo.FATOffset = 1;
    mountInfo.FATSize = bootsector->sectorsPerFAT;
    mountInfo.FATEntrySize = 8;
    mountInfo.rootEntriesCount = bootsector->directoryEntries;
    mountInfo.rootOffset = (bootsector->FATs * bootsector->sectorsPerFAT) + 1;
    mountInfo.rootSize = (bootsector->directoryEntries * 32) / bootsector->bytesPerSector;

    uint8_t *FAT = IDE::the->readSector(1);

    int j = 0;
    int c = 0;
    while(j < 512) {
        uint16_t val = *(uint16_t*)&FAT[j];

        if(c == 0) {
            uint8_t p1 = val;
            uint8_t p2 = val >> 8;

            val = ((0x0f & p2) << 8) | p1;
        } else {
            uint8_t p1 = val;
            uint8_t p2 = val >> 8;

            val = p2 << 4 | ((0xf0 & p1) >> 4);
        }

        sout("0x%x ", val);
        // if(val == 0x0) {
        //     sout("0 ");
        // } else if (val >= 0xFF8){
        //     sout("E ");
        // } else {
        //     sout("1 ");
        // }

        if(c == 1) {
            c = -1;
            j++;
        }
        c++;
        j++;
    }
    sout("\n");

    // Vector<int> sectors = findSectors(1);

}

FILE FAT12::directory(String dirName) {
    FILE file;
    uint8_t* buf;
    PDIRECTORY directory;

    String dosName = "           ";
    String upper = dirName.toUpper();

    for(int i = 0; i < 8; i++) {
        if(upper[i] == '.' && i == upper.length() - 4) break;
        dosName[i] = upper[i];
    }

    for(int i = 0; i < 3; i++) {
        int test = upper.length() - i - 1;
        dosName[10 - i] = upper[test];
    }

    for(int sector = 0; sector < 14; sector++) {
        buf = IDE::the->readSector(mountInfo.rootOffset + sector);

        directory = (PDIRECTORY) buf;

        for(int i = 0; i < 16; i++) {
            String fname((char*)directory->filename);

            if(fname == dosName) {
                memcpy(file.name, dirName.c_str(), dirName.length());

                file.id = 0;
                file.currentCluster = directory->firstClusterLow;
                file.eof = 0;
                file.fileLength = directory->size;

                if(directory->attributes == 0x10) {
                    file.flags = FS_DIRECTORY;
                } else {
                    file.flags = FS_FILE;
                }

                return file;
            }
            directory++;
        }
    }

    file.flags = FS_INVALID;
    return file;
}

void FAT12::read(PFILE file, uint8_t* buf, int len) {
    if(file) {
        uint32_t physSector = 32 + file->currentCluster - 1;

        uint8_t* sector = IDE::the->readSector(physSector);
        memcpy(buf, sector, 512);
        delete sector;

        uint32_t FATOffset = file->currentCluster + (file->currentCluster/2);
        uint32_t FATSector = 1 + (FATOffset / SECTOR_SIZE);
        uint32_t entryOffset = FATOffset % SECTOR_SIZE;

        sector = IDE::the->readSector(FATSector);
        memcpy(FAT, sector, 512);
        delete sector;

        sector = IDE::the->readSector(FATSector + 1);
        memcpy(FAT + SECTOR_SIZE, sector, 512);
        delete sector;

        uint16_t nextCluster = *(uint16_t*)&FAT[entryOffset];

        if(file->currentCluster & 0x0001) {
            nextCluster >>= 4;
        } else {
            nextCluster &= 0x0FFF;
        }

        if(nextCluster >= 0xFF8) {
            file->eof = 1;
            return;
        }

        if(nextCluster == 0) {
			file->eof = 1;
			return;
		}

        file->currentCluster = nextCluster;
    }
}

void FAT12::close(PFILE file) {
    if(file) {
        file->flags = FS_INVALID;
    }
}

FILE FAT12::openSubDir(FILE kFile, String name) {
    FILE file;

    String dosName = "           ";
    String upper = name.toUpper();

    for(int i = 0; i < 8; i++) {
        if(upper[i] == '.' && i == upper.length() - 4) break;
        dosName[i] = upper[i];
    }

    for(int i = 0; i < 3; i++) {
        int test = upper.length() - i - 1;
        dosName[10 - i] = upper[test];
    }

    while(!kFile.eof) {
        uint8_t buf[512];
        read(&file, buf, 512);

        PDIRECTORY pkDir = (PDIRECTORY) buf;

        for(int i = 0; i < 16; i++) {
            String fname((char*)pkDir->filename);

            if(fname == dosName) {
                memcpy(file.name, name.c_str(), name.length());

                file.id = 0;
                file.currentCluster = pkDir->firstClusterLow;
                file.eof = 0;
                file.fileLength = pkDir->size;

                if(pkDir->attributes == 0x10) {
                    file.flags = FS_DIRECTORY;
                } else {
                    file.flags = FS_FILE;
                }

                return file;
            }
            pkDir++;
        }
    }

    file.flags = FS_INVALID;
	return file;
}

FILE FAT12::open(String name) {
    FILE curDirectory;
    bool rootDir = true;
    
    Vector<String> split = name.split('\\');

    if(split.length() == 1) {
        curDirectory = directory(name);

        if(curDirectory.flags == FS_FILE) {
            return curDirectory;
        }

        FILE ret;
        ret.flags = FS_FILE;
        return ret;
    }

    return curDirectory;
}

Vector<String> FAT12::list() {
    Vector<String> ret;
    uint8_t* buf;
    PDIRECTORY directory;

    for(int sector = 0; sector < 14; sector++) {
        buf = IDE::the->readSector(mountInfo.rootOffset + sector);

        directory = (PDIRECTORY) buf;

        for(int i = 0; i < 16; i++) {
            // String fname((char*)directory->filename);
            if(directory->filename[0] != '\0') {
                String fname = (char*)directory->filename;
                Vector<String> split = fname.split(' ');
                if(split.length() == 2) {
                    fname = split[0] + "." + split[1];
                } else {
                    fname = split[0];
                }

                ret.push_back(fname);
            }

            directory++;
        }
    }
    return ret;
}

Vector<int> FAT12::findSectors(int sectorCount) {
    Vector<int> sectors;

    for(int j = 0; j < mountInfo.FATSize; j++) {
        uint8_t* FAT = IDE::the->readSector(mountInfo.FATOffset + j);

        int i = 0;
        int c = 0;
        int sector = 0;
        while(i < 512) {
            uint16_t val = *(uint16_t*)&FAT[i];

            if(c == 0) {
                uint8_t p1 = val;
                uint8_t p2 = val >> 8;

                val = ((0x0f & p2) << 8) | p1;
            } else {
                uint8_t p1 = val;
                uint8_t p2 = val >> 8;

                val = p2 << 4 | ((0xf0 & p1) >> 4);
            }

            if(val == 0x0) {
                sectors.push_back(sector);

                if((int)sectors.size() >= sectorCount) {
                    delete FAT;
                    return sectors;
                }
            }

            if(c == 1) {
                c = -1;
                i++;
            }
            c++;
            i++;
            sector++;
        }

        delete FAT;
    }

    return sectors;
}

void FAT12::write(PFILE file, uint8_t* buf, int len) {
    // file->name;
}

void FAT12::writetest(String filename, uint8_t* buffer, int len) {
    DIRECTORY directory;

    String dosName = "           ";
    String upper = filename.toUpper();

    for(int i = 0; i < 8; i++) {
        if(upper[i] == '.' && i == upper.length() - 4) break;
        dosName[i] = upper[i];
    }

    for(int i = 0; i < 3; i++) {
        int test = upper.length() - i - 1;
        dosName[10 - i] = upper[test];
    }

    Vector<int> sectors;

    if(len % SECTOR_SIZE == 0) {
        sectors = findSectors(len/SECTOR_SIZE);
    } else {
        sectors= findSectors(len/SECTOR_SIZE + 1);
    }

    if(sectors.length() > 0) {
        memcpy(directory.filename, dosName.c_str(), 11);
        directory.attributes = 0x0;
        directory.creationTimeMS = 0;

        // FIX ME: better clock implementation
        directory.creationTime = RTC::the->second/2 + RTC::the->minute * 0x20 + (RTC::the->hour-1) * 0x800;
        directory.creationDate = RTC::the->day + RTC::the->month * 0x20 + (RTC::the->year + 20) * 0x200;
        directory.lastAccessedDate = directory.creationDate;
        directory.firstClusterHi = 0;
        directory.lastModifiedTime = directory.creationTime;
        directory.lastModifiedDate = directory.creationDate;
        directory.firstClusterLow = sectors[0];
        directory.size = len;


        PDIRECTORY dir;
        for(int sector = 0; sector < 14; sector++) {
            uint8_t *buf = IDE::the->readSector(mountInfo.rootOffset + sector);

            dir = (PDIRECTORY) buf;

            for(int i = 0; i < 16; i++) {
                if(dir->filename[0] == '\0') {
                    memcpy(dir, &directory, sizeof(DIRECTORY));

                    IDE::the->writeSector(mountInfo.rootOffset + sector, buf);
                    updateFAT(sectors);
                    for(int j = 0; j < sectors.size(); j++) {
                        uint8_t chunk[512];
                        memcpy(chunk, buffer + (512*j), 512);

                        IDE::the->writeSector(32 + sectors[j] - 1, chunk);
                    }

                    return;
                }
                
                dir++;
            }
        }
    }
}

void FAT12::updateFAT(Vector<int> sectors) {
    for(int i = 0; i < sectors.size(); i++) {
        uint32_t FATOffset = sectors[i] + (sectors[i]/2);
        uint32_t FATSector = 1 + (FATOffset / SECTOR_SIZE);
        uint32_t entryOffset = FATOffset % SECTOR_SIZE;

        uint8_t* FAT = IDE::the->readSector(FATSector);

        uint16_t* entry = (uint16_t*)&FAT[entryOffset];

        sout("0x%x\n", *entry);

        if(sectors[i] & 0x0001) {
            if(i + 1 == sectors.size()) {
                *entry |= 0xFFF0;
            } else {
                int s2 = sectors[i + 1];

                *entry |= 0x10 * s2;
            }
        } else {
            if(i + 1 == sectors.size()) {
                *entry |= 0x0FFF;
            } else {
                int s2 = sectors[i + 1];
                *entry |= s2;
            }
        }

        IDE::the->writeSector(FATSector, FAT);

        // sout("0x%x\n", *entry);
        // sout("0x%x\n", FAT[entryOffset+1]);

        // for(int k = 0; k < 512; k++) {
        //     sout("0x%x ", FAT[k]);
        // }
        // sout("\n");


        // int j = 0;
        // int c = 0;
        // while(j < 512) {
        //     uint16_t val = *(uint16_t*)&FAT[j];

        //     if(c == 0) {
        //         uint8_t p1 = val;
        //         uint8_t p2 = val >> 8;

        //         val = ((0x0f & p2) << 8) | p1;
        //     } else {
        //         uint8_t p1 = val;
        //         uint8_t p2 = val >> 8;

        //         val = p2 << 4 | ((0xf0 & p1) >> 4);
        //     }

        //     sout("0x%x ", val);
        //     // if(val == 0x0) {
        //     //     sout("0 ");
        //     // } else if (val >= 0xFF8){
        //     //     sout("E ");
        //     // } else {
        //     //     sout("1 ");
        //     // }

        //     if(c == 1) {
        //         c = -1;
        //         j++;
        //     }
        //     c++;
        //     j++;
        // }
        // sout("\n");
    }
}