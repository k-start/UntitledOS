#ifndef FS_H
#define FS_H

#include <kernel/Filesystem/VFS.h>

class FS {
    public:

        FILE directory(String name);
        void read(PFILE file, uint8_t* buf, int len);
};
#endif