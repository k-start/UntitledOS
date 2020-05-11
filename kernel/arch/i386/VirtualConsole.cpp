#include <kernel/VirtualConsole.h>
#include <kernel/kstdio.h>

VirtualConsole *VirtualConsole::currentConsole = nullptr;

VirtualConsole::VirtualConsole() : KeyboardListener() {
    VirtualConsole::currentConsole = this;
    vgaBuffer = (uint16_t*) 0xC03FF000;

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            vgaBuffer[x + y*width] = ' ' | 0x0f << 8;
        }
    }
}

void VirtualConsole::keyStateChanged(u8 raw, bool pressed) {
    kprintf("%d", raw);
}

void VirtualConsole::putchar(char c) {
    if(c == '\n') {
        column = 0;
        row++;
    } else {
        unsigned char uc = c;
        
        const int index = row * 80 + column;
	    vgaBuffer[index] = c | 0x0f << 8;

        if (++column == 80) {
            column = 0;
        }
    }
}