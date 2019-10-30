#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

extern "C" {
    /* Defines a GDT entry. We say packed, because it prevents the
    *  compiler from doing things that it thinks is best: Prevent
    *  compiler "optimization" by packing */
    struct gdt_entry
    {
        unsigned short limit_low;
        unsigned short base_low;
        unsigned char base_middle;
        unsigned char access;
        unsigned char granularity;
        unsigned char base_high;
    } __attribute__((packed));

    /* Special pointer which includes the limit: The max bytes
    *  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
    struct gdt_ptr
    {
        unsigned short limit;
        struct gdt_entry *base;
    } __attribute__((packed));

    /* This will be a function in start.asm. We use this to properly
    *  reload the new segment registers */
    extern void _gdt_flush();

    void gdt_install();
    void gdt_set_gate(int, unsigned long, unsigned long, unsigned char, unsigned char);
}

#endif