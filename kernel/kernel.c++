#include "kernel.h"
 
void initialize() {
	for(int c = 0; c < VGA_COLS; c++) {
		for(int r = 0; r < VGA_ROWS; r++) {
			const size_t index = (VGA_COLS * r) + c;
			vgaBuffer[index] = ((uint16_t)color << 8) | ' ';
		}
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
		row = 0;
	}
}

void print(const char* string) {
	for(size_t i = 0; string[i] != '\0'; i++) {
		putChar(string[i]);
	}
}

extern "C" {
    void kernel_main() {
        initialize();

		print("Hello World!\n");
		print("This is a new line!\n");
    }
}