#ifndef KERNEL_CONSOLE_H
#define KERNEL_CONSOLE_H

#include <kernel/devices/KeyboardDevice.h>
#include <Types.h>
#include <stdint.h>
#include <Y/String.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

class VirtualConsole : public KeyboardListener {

    public:
        VirtualConsole();

        static VirtualConsole *currentConsole;

        void updateCursor(int x, int y);
        virtual void keyStateChanged(u8 raw, bool pressed) override;

        void putchar(char c);
        void scroll();

        void newCommand();
        void runCommand(String command);
        void clearCommand();

    private:
        uint16_t* vgaBuffer;
        int row = 0, column = 0;

        String inputStr;
        String previousCommand;

};

#endif