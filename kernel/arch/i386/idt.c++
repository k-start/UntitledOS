#include <kernel/idt.h>

idtGate_t idt[IDT_ENTRIES];
idtRgister_t idtReg;

void setIdtGate(int n, unsigned int handler) {
    idt[n].lowOffset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].zero = 0;
    idt[n].flags = 0x8E; 
    idt[n].highOffset = high_16(handler);
}

void setIdt() {
    idtReg.base = (unsigned int) &idt;
    idtReg.limit = IDT_ENTRIES * sizeof(idtGate_t) - 1;

    idtLoad();
}