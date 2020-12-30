#include <kernel/ports.h>

void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

unsigned char inb(uint16_t port) {
    unsigned char ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

unsigned short ins(uint16_t port) {
	unsigned short ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void insm(uint16_t port, uint8_t* data, unsigned long size) {
	asm volatile ("rep insw" : "+D" (data), "+c" (size) : "d" (port) : "memory");
}

void outsm(uint16_t port, uint8_t* data, unsigned long size) {
	asm volatile ("rep outsw" : "+S" (data), "+c" (size) : "d" (port));
}