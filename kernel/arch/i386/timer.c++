#include <kernel/timer.h>

int ticks = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;

static void timerCallback(registers_t *regs) {
    ticks++;

    if(ticks%100 == 0) {
        seconds++;
        ticks = 0;

        if(seconds == 60) {
            seconds = 0;
            minutes++;

            if(minutes == 60) {
                minutes = 0;
                hours++;
            }
        }

        setTime(hours, minutes, seconds);
    }
}

void initTimer(unsigned int freq) {
    registerInterruptHandler(IRQ0, &timerCallback);

    unsigned int divisor = 1193180 / freq;
    unsigned char low  = (unsigned char)(divisor & 0xFF);
    unsigned char high = (unsigned char)( (divisor >> 8) & 0xFF);

    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
}