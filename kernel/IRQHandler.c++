#include "IRQHandler.h"

IRQHandler::IRQHandler(u8 IRQNumber) : IRQNumber(IRQNumber) {
    registerInterruptHandler(IRQNumber, this);
}

u8 IRQHandler::getIRQNumber() {
    return IRQNumber;
}