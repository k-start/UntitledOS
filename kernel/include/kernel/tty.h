#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdint.h>
#include <stddef.h>

void terminalInitialize(void);
void terminalPutChar(char c);
void terminalWrite(const char* data, size_t size);
void terminalWriteString(const char* data);
void terminalScroll();
void terminalSetColor(uint8_t color);

void setTime(int hours, int minutes, int seconds);

#endif