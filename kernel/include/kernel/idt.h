#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#define KERNEL_CS 0x08
#define IDT_ENTRIES 256
#define low_16(address) (unsigned short)((address) & 0xFFFF)
#define high_16(address) (unsigned short)(((address) & 0xFFFF0000) >> 16)

extern "C" {

    extern void idtLoad();

    typedef struct {
        unsigned short int lowOffset;
        unsigned short int sel;
        unsigned char zero;
        unsigned char flags; 
        unsigned short int highOffset;
    } __attribute__((packed)) idtGate_t ;

    typedef struct {
        unsigned short int limit;
        unsigned int base;
    } __attribute__((packed)) idtRgister_t;

    void setIdtGate(int n, unsigned int handler);
    void setIdt();

}

#endif