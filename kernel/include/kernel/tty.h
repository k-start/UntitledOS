#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

void terminalInitialize(void);
void terminalPutChar(char c);
void terminalWrite(const char* data, size_t size);
void terminalWriteString(const char* data);
void terminalScroll();
void terminalSetColor(uint8_t color);

#endif