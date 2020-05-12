#include <kernel/kstdio.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/CPU.h>
#include <kernel/VirtualConsole.h>
#include <kernel/kstdlib.h>

#include "../devices/KeyboardDevice.h"
#include "../PIT.h"
#include "../RTC.h"

extern "C" {

    void kernel_main(void) {
        // terminalInitialize();

        // printf(" _    _       _   _ _   _          _  ____   _____ \n");
        // printf("| |  | |     | | (_) | | |        | |/ __ \\ / ____|\n");
        // printf("| |  | |_ __ | |_ _| |_| | ___  __| | |  | | (___  \n");
        // printf("| |  | | '_ \\| __| | __| |/ _ \\/ _` | |  | |\\___ \\ \n");
        // printf("| |__| | | | | |_| | |_| |  __/ (_| | |__| |____) |\n");
        // printf(" \\____/|_| |_|\\__|_|\\__|_|\\___|\\__,_|\\____/|_____/ \n\n");
        
        // printf("UntitledOS Kernel\n");

        gdtInstall();
        CPU::isrInstall();

        // enable interrupts
        asm volatile("sti");

        PIT pit(IRQ0, 100);
        KeyboardDevice keyboard(IRQ1);
        VirtualConsole test;
        // RTC rtc(IRQ8);

        // VirtualConsole* pointer = (VirtualConsole*)kmalloc(sizeof(VirtualConsole));
        new VirtualConsole();

        // stops main exiting too soon
        for(;;) {
            asm("hlt");
        }
    }
}