#ifndef _KERNEL_TIMER_H
#define _KERNEL_TIMER_H

#include <kernel/isr.h>
#include <kernel/tty.h>
#include <kernel/ports.h>

void init_timer(unsigned int freq);

#endif