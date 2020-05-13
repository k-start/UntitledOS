#ifndef KERNEL_CONSOLE_H
#define KERNEL_CONSOLE_H

#include <kernel/devices/KeyboardDevice.h>
#include <Types.h>
#include <stdint.h>

class VirtualConsole : public KeyboardListener {

    public:
        VirtualConsole();

        static VirtualConsole *currentConsole;

        virtual void keyStateChanged(u8 raw, bool pressed) override;

        void putchar(char c);
        void scroll();

    private:
        uint16_t* vgaBuffer;
        int row = 0, column = 0;

        const int width = 80, height = 25;
};

#endif