#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/gdt.h>
#include <kernel/isr.h>

extern "C" {

    // extern void InstallGDT();

    void kernel_main(void) {
        gdt_install();
        // InstallGDT();
        terminalInitialize();

        isr_install();

        printf(" _    _       _   _ _   _          _  ____   _____ \n");
        printf("| |  | |     | | (_) | | |        | |/ __ \\ / ____|\n");
        printf("| |  | |_ __ | |_ _| |_| | ___  __| | |  | | (___  \n");
        printf("| |  | | '_ \\| __| | __| |/ _ \\/ _` | |  | |\\___ \\ \n");
        printf("| |__| | | | | |_| | |_| |  __/ (_| | |__| |____) |\n");
        printf(" \\____/|_| |_|\\__|_|\\__|_|\\___|\\__,_|\\____/|_____/ \n\n");
        
        printf("UntitledOS Kernel\n\n");

        __asm__ __volatile__("INT $2");
        __asm__ __volatile__("int $3");

        printf("UntitledOS Kernel\n");
    }
}