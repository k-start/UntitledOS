#include "kernel.h"
 
void initialize() {
	for(int c = 0; c < VGA_COLS; c++) {
		for(int r = 0; r < VGA_ROWS; r++) {
			const size_t index = (VGA_COLS * r) + c;
			vgaBuffer[index] = ((uint16_t)color << 8) | ' ';
		}
	}
}

void scroll() {
	for(int c = 0; c < VGA_COLS; c++) {
		for(int r = 0; r < VGA_ROWS-1; r++) {
			const size_t index = (VGA_COLS * r) + c;
			const size_t nextRowIndex = index + VGA_COLS;
			vgaBuffer[index] = vgaBuffer[nextRowIndex];
		}
	}

	for(int c = 0; c < VGA_COLS; c++) {
		int r = VGA_ROWS-1;
		const size_t index = (VGA_COLS * r) + c;
		vgaBuffer[index] = ((uint16_t)color << 8) | ' ';
	}
}

void putChar(char c) {
	if(c == '\n') {
		column = 0;
		row++;
	} else {
		const size_t index = (VGA_COLS * row) + column;
		vgaBuffer[index] = ((uint16_t)color << 8) | c;
		column++;
	}

	if(column >= VGA_COLS) {
		column = 0;
		row++;
	}

	if(row >= VGA_ROWS) {
		column = 0;
		row--;
		scroll();
	}
}

void print(const char* string, uint8_t textColor = 0x0F, uint8_t backgroundColor = 0x00) {
	color = backgroundColor << 4 | textColor;
	for(size_t i = 0; string[i] != '\0'; i++) {
		putChar(string[i]);
	}
	color = 0x0F;
}

extern "C" {
    void kernel_main() {
        initialize();

		print("Hello World!\n");
		print("This is a new line!\n");
		print("This line has colour\n", LIGHT_BLUE);
		print("This line has a different colour\n", LIGHT_RED);
		print("This line has a background colour\n", BLACK, WHITE);
    }
}