#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>

// #include <kernel/Filesystem/FS.h>
#include <Y/String.h>

#define FS_FILE       0
#define FS_DIRECTORY  1
#define FS_INVALID    2

#define DEVICE_MAX 26

#define FILE VFS::FILE_
#define PFILE VFS::PFILE_

class FAT12;
class VFS {

    public:

        typedef struct _FILE {
            char name[32];
            uint32_t flags;
            uint32_t fileLength;
            uint32_t id;
            uint32_t eof;
            uint32_t position;
            uint32_t currentCluster;
            uint32_t device;
        } FILE_, *PFILE_;

        VFS();

        FILE openFile(const char* fname);
        void readFile(PFILE file, unsigned char* buffer, size_t length);
        void closeFile(PFILE file);
        Vector<String> list(unsigned int deviceID);
        void registerFileSystem(FAT12* fsys, unsigned int deviceID);
        void unregisterFileSystem(FAT12* fsys);
        void unregisterFileSystemByID(unsigned int deviceID);

        void write(String filename, String content);

        static VFS *the;

    private:

        // abstract out in future
        FAT12* _FileSystems[DEVICE_MAX];

};

#endif