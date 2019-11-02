#include <kernel/tty.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xC03FF000;

static size_t terminalRow;
static size_t terminalColumn;
static uint8_t terminalColor;
static uint16_t* terminalBuffer;

void update_cursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminalInitialize(void) {
    terminalRow = 0;
    terminalColumn = 0;
    terminalColor = vgaEntryColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminalBuffer = VGA_MEMORY;
    for(size_t y = 0; y < VGA_HEIGHT; y++) {
        for(size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
			terminalBuffer[index] = vgaEntry(' ', terminalColor);
        }
    }
}

void terminalSetColor(uint8_t color) {
    terminalColor = color;
}

void terminalPutEntryAt(unsigned char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
	terminalBuffer[index] = vgaEntry(c, color);
}

void terminalPutChar(char c) {
    if(c == '\n') {
        terminalColumn = 0;
        terminalRow++;
    } else {
        unsigned char uc = c;
        terminalPutEntryAt(uc, terminalColor, terminalColumn, terminalRow);
        if (++terminalColumn == VGA_WIDTH) {
            terminalColumn = 0;
        }
    }

    if(terminalRow == VGA_HEIGHT) {
        terminalRow--;
        terminalScroll();
    }

    update_cursor(terminalColumn, terminalRow);
}

void terminalWrite(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
		terminalPutChar(data[i]);
}

void terminalWriteString(const char* data) {
	terminalWrite(data, strlen(data));
}

void terminalScroll() {
    for(size_t y = 0; y < VGA_HEIGHT-1; y++) {
        for(size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
			terminalBuffer[index] = terminalBuffer[index + VGA_WIDTH];
        }
    }
    for(size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = (VGA_HEIGHT-1) * VGA_WIDTH + x;
        terminalBuffer[index] = vgaEntry(' ', terminalColor);
    }
}

void setTime(int hours, int minutes, int seconds) {
    char secondsStr[3], minutesStr[3], hoursStr[3];
    itoa(seconds, secondsStr);
    itoa(minutes, minutesStr);
    itoa(hours, hoursStr);

    if(seconds < 10) {
        terminalPutEntryAt('0', terminalColor, 14, 8);
        terminalPutEntryAt(secondsStr[0], terminalColor, 15, 8);
    } else {
        terminalPutEntryAt(secondsStr[0], terminalColor, 14, 8);
        terminalPutEntryAt(secondsStr[1], terminalColor, 15, 8);
    }

    if(minutes < 10) {
        terminalPutEntryAt('0', terminalColor, 11, 8);
        terminalPutEntryAt(minutesStr[0], terminalColor, 12, 8);
    } else {
        terminalPutEntryAt(minutesStr[0], terminalColor, 11, 8);
        terminalPutEntryAt(minutesStr[1], terminalColor, 12, 8);
    }

    if(hours < 10) {
        terminalPutEntryAt('0', terminalColor, 11, 8);
        terminalPutEntryAt(hoursStr[0], terminalColor, 8, 8);
    } else {
        terminalPutEntryAt(hoursStr[0], terminalColor, 8, 8);
        terminalPutEntryAt(hoursStr[1], terminalColor, 9, 8);
    }
}