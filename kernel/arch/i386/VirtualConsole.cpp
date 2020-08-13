#include <kernel/VirtualConsole.h>
#include <kernel/kstdio.h>
#include <kernel/ports.h>
#include <kernel/pmm.h>
#include <string.h>
#include <kernel/RTC.h>
#include <kernel/PIT.h>

static char map[0x80] = {
    0, '\033', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0x08, '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, '*', 0, ' ', 0, 0,
    //60
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    //70
    0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
    //80
    0, 0, 0, 0, 0, 0, '\\', 0, 0, 0,
};

static char shift_map[0x80] = {
    0, '\033', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0x08, '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0, '*', 0, ' ', 0, 0,
    //60
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    //70
    0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
    //80
    0, 0, 0, 0, 0, 0, '|', 0, 0, 0,
};

VirtualConsole *VirtualConsole::currentConsole = nullptr;

VirtualConsole::VirtualConsole() : KeyboardListener() {
    VirtualConsole::currentConsole = this;
    // vgaBuffer = (uint16_t*) 0xC03FF000;
    vgaBuffer = (uint16_t*) 0xC00B8000;

    for(int x = 0; x < VGA_WIDTH; x++) {
        for(int y = 0; y < VGA_HEIGHT; y++) {
            vgaBuffer[x + y*VGA_WIDTH] = ' ' | 0x0f << 8;
        }
    }
}

void VirtualConsole::updateCursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void VirtualConsole::keyStateChanged(u8 raw, bool pressed) {
    u8 ch = raw & 0x7f;

    bool shifted = KeyboardDevice::keyboardDevice->shifted;
    bool capsLock = KeyboardDevice::keyboardDevice->capsLock;

    if(pressed) {
        // sout("0x%x\n", ch);
        switch (ch) {
            case 0x0E:
                if(inputStr.length() > 0) {
                    inputStr.pop_back();
                    column--;
                    putchar(' ');
                    column--;
                    updateCursor(column, row);
                }
                break;
            case 0x1C:
                runCommand(inputStr);
                newCommand();
                break;
            case 0x2A: case 0x1D: case 0x3A: case 0x0F: case 0x36: case 0x38: case 0x5C:
                
                break;
            // up arrow
            case 0x48:
                if(commandIndex > 0) {
                    clearCommand();
                    commandIndex--;
                    inputStr = previousCommands[commandIndex];
                    printf("%s", inputStr.c_str());
                }
                break;
            // down arrow
            case 0x50:
                if(commandIndex < previousCommands.size() - 1) {
                    clearCommand();
                    commandIndex++;
                    inputStr = previousCommands[commandIndex];
                    printf("%s", inputStr.c_str());
                }
                break;
            case 0x4B: 
                break;
            case 0x4D:
                break;
            // escape
            case 0x01:
                clearCommand();
                break;
            default:
                char c = (shifted | capsLock) ? shift_map[ch] : map[ch];
                inputStr.append(c);
                putchar(c);
        }
    }
}

void VirtualConsole::putchar(char c) {
    if(c == '\n') {
        column = 0;
        row++;
    } else {        
        const int index = row * VGA_WIDTH + column;
	    vgaBuffer[index] = c | 0x0f << 8;

        if (++column == VGA_WIDTH) {
            column = 0;
            row++;
        }
    }

    if(row == VGA_HEIGHT) {
        row--;
        scroll();
    }

    updateCursor(column, row);
}

void VirtualConsole::scroll() {
    for(int y = 0; y < VGA_HEIGHT-1; y++) {
        for(int x = 0; x < VGA_WIDTH; x++) {
            const int index = y * VGA_WIDTH + x;
			vgaBuffer[index] = vgaBuffer[index + VGA_WIDTH];
        }
    }
    for(int x = 0; x < VGA_WIDTH; x++) {
        const int index = (VGA_HEIGHT-1) * VGA_WIDTH + x;
        vgaBuffer[index] = ' ' | 0x0f << 8;
    }
}

void VirtualConsole::newCommand() {
    inputStr.clear();
    row++;
    if(row == VGA_HEIGHT) {
        row--;
        scroll();
    }
    column = 0;
    printf("> ");
    updateCursor(column, row);
}

void VirtualConsole::runCommand(String command) {
    if(command.length() > 0) {
        previousCommands.push_back(command);
        commandIndex = previousCommands.size();

        // switch (command) {
        //     case "time":
        //     sout("test\n");
        //     break;
        // }

        printf("\n");
        if(command == "time") {
            printf(RTC::the->getTime().c_str());
        } else if(command == "date") {
            printf(RTC::the->getDate().c_str());
        } else if(command == "uptime") {
            printf(PIT::the->getUptimeStr().c_str());
        }  else if(command == "ram") {
            printf("%d mb", PMM::the->memorySize/1024 + 1);
        } else if(command == "mem") {
            printf("Size: %d kb\n", PMM::the->memorySize);
            printf("Max Blocks: %d\n", PMM::the->maxBlocks);
            printf("Used Blocks: %d\n", PMM::the->usedBlocks);
            printf("Free Blocks: %d", PMM::the->maxBlocks - PMM::the->usedBlocks);
        } else {
            printf("Unknown command: %s", command.c_str());
        }
    }
}

void VirtualConsole::clearCommand() {    
    sout("%d\n", inputStr.length());
    for(int i = 0; i < inputStr.length(); i++) {
        column--;
        putchar(' ');
        column--;
    }

    inputStr = "";

    updateCursor(column, row);
}