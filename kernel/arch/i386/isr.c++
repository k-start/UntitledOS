#include <kernel/isr.h>
#include <kernel/idt.h>

#include "../../IRQHandler.h"

IRQHandler *interruptHandlers[256];

/* Can't do this with a loop because we need the address
 * of the function names */
void isrInstall() {
    setIdtGate(0, (unsigned int)isr0);
    setIdtGate(1, (unsigned int)isr1);
    setIdtGate(2, (unsigned int)isr2);
    setIdtGate(3, (unsigned int)isr3);
    setIdtGate(4, (unsigned int)isr4);
    setIdtGate(5, (unsigned int)isr5);
    setIdtGate(6, (unsigned int)isr6);
    setIdtGate(7, (unsigned int)isr7);
    setIdtGate(8, (unsigned int)isr8);
    setIdtGate(9, (unsigned int)isr9);
    setIdtGate(10, (unsigned int)isr10);
    setIdtGate(11, (unsigned int)isr11);
    setIdtGate(12, (unsigned int)isr12);
    setIdtGate(13, (unsigned int)isr13);
    setIdtGate(14, (unsigned int)isr14);
    setIdtGate(15, (unsigned int)isr15);
    setIdtGate(16, (unsigned int)isr16);
    setIdtGate(17, (unsigned int)isr17);
    setIdtGate(18, (unsigned int)isr18);
    setIdtGate(19, (unsigned int)isr19);
    setIdtGate(20, (unsigned int)isr20);
    setIdtGate(21, (unsigned int)isr21);
    setIdtGate(22, (unsigned int)isr22);
    setIdtGate(23, (unsigned int)isr23);
    setIdtGate(24, (unsigned int)isr24);
    setIdtGate(25, (unsigned int)isr25);
    setIdtGate(26, (unsigned int)isr26);
    setIdtGate(27, (unsigned int)isr27);
    setIdtGate(28, (unsigned int)isr28);
    setIdtGate(29, (unsigned int)isr29);
    setIdtGate(30, (unsigned int)isr30);
    setIdtGate(31, (unsigned int)isr31);

    // Remap the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0); 

    // Install the IRQs
    setIdtGate(32, (unsigned int)irq0);
    setIdtGate(33, (unsigned int)irq1);
    setIdtGate(34, (unsigned int)irq2);
    setIdtGate(35, (unsigned int)irq3);
    setIdtGate(36, (unsigned int)irq4);
    setIdtGate(37, (unsigned int)irq5);
    setIdtGate(38, (unsigned int)irq6);
    setIdtGate(39, (unsigned int)irq7);
    setIdtGate(40, (unsigned int)irq8);
    setIdtGate(41, (unsigned int)irq9);
    setIdtGate(42, (unsigned int)irq10);
    setIdtGate(43, (unsigned int)irq11);
    setIdtGate(44, (unsigned int)irq12);
    setIdtGate(45, (unsigned int)irq13);
    setIdtGate(46, (unsigned int)irq14);
    setIdtGate(47, (unsigned int)irq15);

    setIdt(); // Load with ASM
}

/* To print the message which defines every exception */
char exceptionMessages[35][50] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt\0",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isrHandler(registers_t *r) {
    terminalSetColor(0x0C);
    printf("Interrupt Recieved: %s\n", exceptionMessages[r->int_no]);
    terminalSetColor(0x0F);
}

void registerInterruptHandler(unsigned char n, void *handler) {
    interruptHandlers[n] = (IRQHandler*)handler;
}

void irqHandler(registers_t *r) {
    if (r->int_no >= 40) outb(0xA0, 0x20); 
    outb(0x20, 0x20);

    if (interruptHandlers[r->int_no] != nullptr) {
        IRQHandler *handler = interruptHandlers[r->int_no];
        handler->handleIRQ();
    }
}