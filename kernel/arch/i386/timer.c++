#include <kernel/timer.h>

static void timer_callback(registers_t *regs) {
    // printf("Tick\n");
}

void init_timer(unsigned int freq) {
    registerInterruptHandler(IRQ0, &timer_callback);

    unsigned int divisor = 1193180 / freq;
    unsigned char low  = (unsigned char)(divisor & 0xFF);
    unsigned char high = (unsigned char)( (divisor >> 8) & 0xFF);

    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
}