#include <kernel/Filesystem/VFS.h>
#include <string.h>

#include <kernel/Filesystem/FAT12.h>

VFS *VFS::the;

VFS::VFS() {
    the = this;

    FAT12 *fat12 = new FAT12();
    registerFileSystem(fat12, 0);
}

VFS::FILE VFS::openFile(const char* fname) {
    if(fname) {
        unsigned char device = 'a';

        char *filename = (char*)fname;

        if(fname[1] == ':') {
            device = fname[0];
            filename += 3;
        }

        if(_FileSystems[device - 'a']) {
            VFS::FILE file = _FileSystems[device - 'a']->open(filename);
            file.device = device;
            return file;
        }

    }

    VFS::FILE file;
    file.flags = FS_INVALID;
    return file;
}

void VFS::readFile(PFILE file, unsigned char* buffer, size_t length) {
    if(file) {
        if(_FileSystems[file->device - 'a']) {
            _FileSystems[file->device - 'a']->read(file, buffer, length);
        }
    }
}

void VFS::closeFile(PFILE file) {
    if(file) {
        if(_FileSystems[file->device - 'a']) {
            _FileSystems[file->device - 'a']->close(file);
        }
    }
}

Vector<String> VFS::list(unsigned int deviceID) {
    if (deviceID < DEVICE_MAX) {
		return _FileSystems[deviceID]->list();
    }
    return Vector<String>();
}

void VFS::registerFileSystem(FAT12* fsys, unsigned int deviceID) {
    if(deviceID < DEVICE_MAX) {
        if(fsys) {
            _FileSystems[deviceID] = fsys;
        }
    }
}

void VFS::unregisterFileSystem(FAT12* fsys) {
    for(int i = 0; i < DEVICE_MAX; i++) {
        if(_FileSystems[i] == fsys) {
            _FileSystems[i] = NULL;
        }
    }
}

void VFS::unregisterFileSystemByID (unsigned int deviceID) {
	if (deviceID < DEVICE_MAX) {
		_FileSystems[deviceID] = NULL;
    }
}

void VFS::write(String filename, String content) {
    unsigned char device = 'a';

    if(filename[1] == ':') {
        device = filename[0];
        filename += 3;
    }

    if(_FileSystems[device - 'a']) {
        _FileSystems[device - 'a']->writetest(filename, (uint8_t*)content.c_str(), content.length());
    }
}