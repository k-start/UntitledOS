#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>

#define FS_FILE       0
#define FS_DIRECTORY  1
#define FS_INVALID    2

#define DEVICE_MAX 26

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
        } FILE, *PFILE;

        typedef struct _FILE_SYSTEM {
            char Name [8];
            FILE (*Directory) (const char* DirectoryName);
            void (*Mount) ();
            void (*Read) (PFILE file, unsigned char* Buffer, size_t Length);
            void (*Close) (PFILE);
            FILE (*Open) (const char* FileName);
        } FILESYSTEM, *PFILESYSTEM;

        VFS();

        FILE openFile(const char* fname);
        void readFile(PFILE file, unsigned char* buffer, size_t length);
        void closeFile(PFILE file);
        void registerFileSystem(PFILESYSTEM fsys, unsigned int deviceID);
        void unregisterFileSystem(PFILESYSTEM fsys);
        void unregisterFileSystemByID(unsigned int deviceID);

        static VFS *the;

    private:
        PFILESYSTEM _FileSystems[DEVICE_MAX];

};

#endif