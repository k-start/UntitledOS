#include <kernel/PIT.h>

#include <stdio.h>
#include <kernel/ports.h>
#include <kernel/kstdio.h>

PIT *PIT::the;

PIT::PIT(u8 IRQNumber, int freq) : IRQHandler(IRQNumber) {
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);

    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);

    the = this;
}

void PIT::handleIRQ() {
    ticks++;
    if(ticks%100 == 0) {
        seconds++;
    }

    if(sleeping) {
        sleepMS += 10;
    }
}

String PIT::getUptimeStr() {
    String string;
    string += seconds;
    return string;
}

// FIX-ME VERY BAD IMPLEMENTATION
void PIT::sleep(int ms) {
    sleepMS = 0;
    sleeping = true;

    while(sleepMS < ms) {
        sout("");
    }
    sout("slept\n");
}