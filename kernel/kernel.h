#ifndef KERNEL_H
#define KERNEL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

const int VGA_COLS = 80;
const int VGA_ROWS = 25;

volatile uint16_t* vgaBuffer = (uint16_t*)0xB8000;

int column = 0;
int row = 0;
uint8_t color = 0x0F;

#endif