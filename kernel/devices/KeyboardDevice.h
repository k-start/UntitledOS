#ifndef KEYBOARDDEVICE_H
#define KEYBOARDDEVICE_H

#include <Types.h>
#include "../IRQHandler.h"

class KeyboardDevice : public IRQHandler {

    public:
        KeyboardDevice(u8 IRQNumber);

        virtual void handleIRQ() override;
        void keyStateChanged(u8 ch, bool pressed);

    private:
        bool shifted = false;
        bool capsLock = false;
};

#endif