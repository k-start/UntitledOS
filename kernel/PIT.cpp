#include <kernel/PIT.h>

#include <stdio.h>
#include <kernel/ports.h>

PIT::PIT(u8 IRQNumber, int freq) : IRQHandler(IRQNumber) {
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);

    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
}

void PIT::handleIRQ() {
    ticks++;
    if(ticks%100 == 0) {
        // 1 second passed
    }
}