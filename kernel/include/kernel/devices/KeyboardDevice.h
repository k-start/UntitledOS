#ifndef KEYBOARDDEVICE_H
#define KEYBOARDDEVICE_H

#include <Types.h>
#include <kernel/IRQHandler.h>

class KeyboardListener {

    public:
        KeyboardListener();
        virtual void keyStateChanged(u8 raw, bool pressed) { };
};

class KeyboardDevice : public IRQHandler {

    public:
        KeyboardDevice(u8 IRQNumber);

        static KeyboardDevice *keyboardDevice;

        virtual void handleIRQ() override;
        void keyStateChanged(u8 ch, bool pressed);

        void registerListener(KeyboardListener *listener);

        bool shifted = false;
        bool capsLock = false;

    private:
        // FIX change to vector once implemented
        KeyboardListener *listeners;
};

#endif