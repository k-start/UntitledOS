#ifndef _KERNEL_PORTS_H
#define _KERNEL_PORTS_H

#include <stdint.h>

void outb(uint16_t port, uint8_t val);
unsigned char inb(uint16_t port);

unsigned short ins(uint16_t port);
void insm(uint16_t port, uint8_t* data, unsigned long size);
void outsm(uint16_t port, uint8_t* data, unsigned long size);

#endif