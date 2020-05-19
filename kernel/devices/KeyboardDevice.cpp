#include <kernel/devices/KeyboardDevice.h>
#include <stdio.h>
#include <kernel/ports.h>

KeyboardDevice *KeyboardDevice::keyboardDevice = nullptr;

KeyboardDevice::KeyboardDevice(u8 IRQNumber) : IRQHandler(IRQNumber) {
    keyboardDevice = this;
}

void KeyboardDevice::handleIRQ() {
    u8 raw = inb(0x60);
    u8 ch = raw & 0x7f;
    bool pressed = !(raw & 0x80);

    // printf("0x%x\n", ch);
    
    if(raw == 0x3A) {
        capsLock = !capsLock;
    }

    keyStateChanged(ch, pressed);
    listeners->keyStateChanged(raw, pressed);
}

void KeyboardDevice::keyStateChanged(u8 ch, bool pressed) {
    switch (ch) {
        case 0x2a:
            shifted = pressed;
            break;
    }

    // if(pressed) {
    //     printf("%c", (shifted | capsLock) ? shift_map[ch] : map[ch]);
    // }
}

void KeyboardDevice::registerListener(KeyboardListener *listener) {
    listeners = listener;
}




KeyboardListener::KeyboardListener() {
    KeyboardDevice::keyboardDevice->registerListener(this);
}