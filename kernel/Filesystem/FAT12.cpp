#include <kernel/Filesystem/FAT12.h>
#include <kernel/kstdio.h>
#include <kernel/Filesystem/IDE.h>
#include <string.h>
#include <Y/Vector.h>

FAT12::FAT12() {
    // sout("%d\n", sizeof(BOOTSECTOR));

    bootsector = (PBOOTSECTOR)IDE::the->readSector(0);

    mountInfo.sectorsCount = bootsector->sectorsCount;
    mountInfo.FATOffset = 1;
    mountInfo.FATSize = bootsector->sectorsPerFAT;
    mountInfo.FATEntrySize = 8;
    mountInfo.rootEntriesCount = bootsector->directoryEntries;
    mountInfo.rootOffset = (bootsector->FATs * bootsector->sectorsPerFAT) + 1;
    mountInfo.rootSize = (bootsector->directoryEntries * 32) / bootsector->bytesPerSector;
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

        if(nextCluster >= 0xff8) {
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