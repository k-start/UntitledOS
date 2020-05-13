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
    // kprintf("%d", raw);
}

void VirtualConsole::putchar(char c) {
    if(c == '\n') {
        column = 0;
        row++;
    } else {        
        const int index = row * 80 + column;
	    vgaBuffer[index] = c | 0x0f << 8;

        if (++column == width) {
            column = 0;
            row++;
        }
    }

    if(row == height) {
        row--;
        scroll();
    }
}

void VirtualConsole::scroll() {
    for(int y = 0; y < height-1; y++) {
        for(int x = 0; x < width; x++) {
            const int index = y * width + x;
			vgaBuffer[index] = vgaBuffer[index + width];
        }
    }
    for(int x = 0; x < width; x++) {
        const int index = (height-1) * width + x;
        vgaBuffer[index] = ' ' | 0x0f << 8;
    }
}