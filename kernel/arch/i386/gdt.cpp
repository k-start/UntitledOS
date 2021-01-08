#include <kernel/gdt.h>
#include <string.h>
#include <kernel/kstdio.h>

struct gdt_entry gdt[6];
struct gdt_ptr gp;

struct TssEntry tss_entry;

void gdtSetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdtInstall() {
    gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
    gp.base = (gdt_entry*)&gdt;

    gdtSetGate(0, 0, 0, 0, 0);

    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    writeTss(5, 0x10, 0x0);

    _gdt_flush();
    _tss_flush();
}

void writeTss(int num, uint16_t ss0, uint32_t esp0) {
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    gdtSetGate(num, base, limit, 0xE9, 0x00);

    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = 0x0b;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void setKernelStack(uint32_t stack) {
    tss_entry.esp0 = stack;
}