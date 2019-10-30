#ifndef IDT_H
#define IDT_H

/* Segment selectors */
#define KERNEL_CS 0x08

extern "C" {

    extern void _idt_load();

    /* How every interrupt gate (handler) is defined */
    typedef struct {
        unsigned short int low_offset; /* Lower 16 bits of handler function address */
        unsigned short int sel; /* Kernel segment selector */
        unsigned char always0;
        /* First byte
        * Bit 7: "Interrupt is present"
        * Bits 6-5: Privilege level of caller (0=kernel..3=user)
        * Bit 4: Set to 0 for interrupt gates
        * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
        unsigned char flags; 
        unsigned short int high_offset; /* Higher 16 bits of handler function address */
    } __attribute__((packed)) idt_gate_t ;

    /* A pointer to the array of interrupt handlers.
    * Assembly instruction 'lidt' will read it */
    typedef struct {
        unsigned short int limit;
        unsigned int base;
    } __attribute__((packed)) idt_register_t;

    #define IDT_ENTRIES 256

    /* Functions implemented in idt.c */
    void set_idt_gate(int n, unsigned int handler);
    void set_idt();

    #define low_16(address) (unsigned short)((address) & 0xFFFF)
    #define high_16(address) (unsigned short)(((address) & 0xFFFF0000) >> 16)

}

#endif