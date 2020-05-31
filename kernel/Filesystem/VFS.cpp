#include <kernel/Filesystem/VFS.h>

VFS *VFS::the;

VFS::VFS() {
    the = this;
}

VFS::FILE VFS::openFile(const char* fname) {
    if(fname) {
        unsigned char device = 'a';

        char *filename = (char*)fname;

        if(fname[1] == ':') {
            device = fname[0];
            filename += 2;
        }

        if(_FileSystems[device - 'a']) {
            VFS::FILE file = _FileSystems[device - 'a']->Open(filename);
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
            _FileSystems[file->device - 'a']->Read(file, buffer, length);
        }
    }
}

void VFS::closeFile(PFILE file) {
    if(file) {
        if(_FileSystems[file->device - 'a']) {
            _FileSystems[file->device - 'a']->Close(file);
        }
    }
}

void VFS::registerFileSystem(PFILESYSTEM fsys, unsigned int deviceID) {
    if(deviceID < DEVICE_MAX) {
        if(fsys) {
            _FileSystems[deviceID] = fsys;
        }
    }
}

void VFS::unregisterFileSystem(PFILESYSTEM fsys) {
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