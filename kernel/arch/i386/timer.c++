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

static void rtcCallback(registers_t *regs) {
    // printf("rtc");
    // outb(0x70, 0x0C);	// select register C
    // inb(0x71);		// just throw away contents

    unsigned char second, minute, hour, day, month, year;

    outb(0x70, 0x07);
    day = inb(0x71);
    day = (day & 0x0F) + ((day / 16) * 10);
    outb(0x70, 0x08);
    month = inb(0x71);
    month = (month & 0x0F) + ((month / 16) * 10);
    outb(0x70, 0x09);
    year = inb(0x71);
    year = (year & 0x0F) + ((year / 16) * 10);

    outb(0x70, 0x00);
    second = inb(0x71);
    second = (second & 0x0F) + ((second / 16) * 10);
    outb(0x70, 0x02);
    minute = inb(0x71);
    minute = (minute & 0x0F) + ((minute / 16) * 10);
    outb(0x70, 0x04);
    hour = inb(0x71);
    hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
    
    printf("%d/%d/%d\n", day, month, year);
    printf("%d:%d:%d\n", hour, minute, second);
}

void initTimer(unsigned int freq) {
    registerInterruptHandler(IRQ0, &timerCallback);
    registerInterruptHandler(IRQ8, &rtcCallback);

    asm("cli");
    outb(0x70, 0x8A);	// select Status Register A, and disable NMI (by setting the 0x80 bit)
    // outb(0x71, 0x20);	// write to CMOS/RTC RAM
    asm("sti");

    asm("cli");
    outb(0x70, 0x8B);		// select register B, and disable NMI
    char prev=inb(0x71);	// read the current value of register B
    outb(0x70, 0x8B);		// set the index again (a read will reset the index to register D)
    outb(0x71, prev | 0x40);
    asm("sti");





    unsigned int divisor = 1193180 / freq;
    unsigned char low  = (unsigned char)(divisor & 0xFF);
    unsigned char high = (unsigned char)( (divisor >> 8) & 0xFF);

    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);
}