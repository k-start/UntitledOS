#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

extern "C" {
    
    struct gdt_entry
    {
        unsigned short limit_low;
        unsigned short base_low;
        unsigned char base_middle;
        unsigned char access;
        unsigned char granularity;
        unsigned char base_high;
    } __attribute__((packed));

    struct gdt_ptr
    {
        unsigned short limit;
        struct gdt_entry *base;
    } __attribute__((packed));

    extern void _gdt_flush();

    void gdt_install();
    void gdt_set_gate(int, unsigned long, unsigned long, unsigned char, unsigned char);
}

#endif