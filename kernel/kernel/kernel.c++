#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/isr.h>
#include <kernel/timer.h>
#include <kernel/keyboard.h>

extern "C" {

    void kernel_main(void) {
        terminalInitialize();

        printf(" _    _       _   _ _   _          _  ____   _____ \n");
        printf("| |  | |     | | (_) | | |        | |/ __ \\ / ____|\n");
        printf("| |  | |_ __ | |_ _| |_| | ___  __| | |  | | (___  \n");
        printf("| |  | | '_ \\| __| | __| |/ _ \\/ _` | |  | |\\___ \\ \n");
        printf("| |__| | | | | |_| | |_| |  __/ (_| | |__| |____) |\n");
        printf(" \\____/|_| |_|\\__|_|\\__|_|\\___|\\__,_|\\____/|_____/ \n\n");
        
        printf("UntitledOS Kernel\n");
        // printf("Uptime: 00:00:00\n");

        gdtInstall();
        isrInstall();

        // enable interrupts
        asm volatile("sti");
        initTimer(100);
        initKeyboard();

        // stops main exiting too soon
        for(;;) {
            asm("hlt");
        }
    }
}