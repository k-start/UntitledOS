#include <kernel/RTC.h>

#include <stdio.h>
#include <kernel/ports.h>

RTC *RTC::the;

RTC::RTC(u8 IRQNumber) : IRQHandler(IRQNumber) {
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

    the = this;
}

void RTC::handleIRQ() {
    outb(0x70, 0x0C);	// select register C
    inb(0x71);		// just throw away contents

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

    // printf("%d/%d/%d\n", day, month, year);
    // printf("%d:%d:%d\n", hour-1, minute, second);
}

String RTC::getTime() {
    String time;
    if(hour-2 < 10 && hour-2 != 0) {
        time += 0;
    }
    time += (hour-2) == 0 ? 12 : (hour-2);
    time += ':';
    if(minute < 10) {
        time += 0;
    }
    time += minute;
    time += ':';
    if(second < 10) {
        time += 0;
    }
    time += second;
    return time;
}

String RTC::getDate() {
    String date;
    if(day < 10) {
        date += 0;
    }
    date += day;
    date += '/';
    if(month < 10) {
        date += 0;
    }
    date += month;
    date += '/';
    if(year < 10) {
        date += 0;
    }
    date += year;
    return date;
}