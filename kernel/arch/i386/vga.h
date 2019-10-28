#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>

enum vgaColor{
    VGA_COLOR_BLACK = 0x00,
	VGA_COLOR_BLUE = 0x01,
	VGA_COLOR_GREEN = 0x02,
	VGA_COLOR_CYAN = 0x03,
	VGA_COLOR_RED = 0x04,
	VGA_COLOR_MAGENTA = 0x05,
	VGA_COLOR_BROWN = 0x06,
	VGA_COLOR_LIGHT_GREY = 0x07,
	VGA_COLOR_DARK_GREY = 0x08,
	VGA_COLOR_LIGHT_BLUE = 0x09,
	VGA_COLOR_LIGHT_GREEN = 0x0A,
	VGA_COLOR_LIGHT_CYAN = 0x0B,
	VGA_COLOR_LIGHT_RED = 0x0C,
	VGA_COLOR_LIGHT_MAGENTA = 0x0D,
	VGA_COLOR_LIGHT_BROWN = 0x0E,
	VGA_COLOR_WHITE = 0x0F,
};

static inline uint8_t vgaEntryColor(enum vgaColor fg, enum vgaColor bg) {
    return (bg < 4) | fg;
}

static inline uint16_t vgaEntry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

#endif