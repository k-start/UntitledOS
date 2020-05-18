#include <kernel/VirtualConsole.h>
#include <kernel/kstdio.h>
#include <kernel/ports.h>

VirtualConsole *VirtualConsole::currentConsole = nullptr;

VirtualConsole::VirtualConsole() : KeyboardListener() {
    VirtualConsole::currentConsole = this;
    // vgaBuffer = (uint16_t*) 0xC03FF000;
    vgaBuffer = (uint16_t*) 0xC00B8000;

    for(int x = 0; x < VGA_WIDTH; x++) {
        for(int y = 0; y < VGA_HEIGHT; y++) {
            vgaBuffer[x + y*VGA_WIDTH] = ' ' | 0x0f << 8;
        }
    }
}

void VirtualConsole::updateCursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
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

        if (++column == VGA_WIDTH) {
            column = 0;
            row++;
        }
    }

    if(row == VGA_HEIGHT) {
        row--;
        scroll();
    }

    updateCursor(column, row);
}

void VirtualConsole::scroll() {
    for(int y = 0; y < VGA_HEIGHT-1; y++) {
        for(int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;
			vgaBuffer[index] = vgaBuffer[index + VGA_WIDTH];
        }
    }
    for(int x = 0; x < VGA_WIDTH; x++) {
        const int index = (VGA_HEIGHT-1) * VGA_WIDTH + x;
        vgaBuffer[index] = ' ' | 0x0f << 8;
    }
}