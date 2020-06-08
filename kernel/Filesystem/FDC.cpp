#include <kernel/Filesystem/FDC.h>
#include <kernel/kstdio.h>

FDC::FDC(u8 IRQNumber) : IRQHandler(IRQNumber) {

}

void FDC::handleIRQ() {
    sout("FDC\n");
}