#include "KeyboardDevice.h"

#include <stdio.h>
#include <kernel/ports.h>

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

KeyboardDevice *KeyboardDevice::keyboardDevice = nullptr;

KeyboardDevice::KeyboardDevice(u8 IRQNumber) : IRQHandler(IRQNumber) {
    keyboardDevice = this;
}

void KeyboardDevice::handleIRQ() {
    u8 raw = inb(0x60);
    u8 ch = raw & 0x7f;
    bool pressed = !(raw & 0x80);

    // printf("%d\n", raw);
    
    if(raw == 0x3A) {
        capsLock = !capsLock;
    }

    keyStateChanged(ch, pressed);
    listeners->keyStateChanged(raw, pressed);

    // if(map[scancode]) {
    //     printf("%d\n", scancode);
    // }
}

void KeyboardDevice::keyStateChanged(u8 ch, bool pressed) {
    // printf("%d %d\n", ch, pressed);
    
    switch (ch) {
        case 0x2a:
            shifted = pressed;
            break;
    }

    if(pressed) {
        // printf("%c", (shifted | capsLock) ? shift_map[ch] : map[ch]);
    }
}

void KeyboardDevice::registerListener(KeyboardListener *listener) {
    listeners = listener;
}



KeyboardListener::KeyboardListener() {
    KeyboardDevice::keyboardDevice->registerListener(this);
}