#include <kernel/kstdio.h>
#include <kernel/VirtualConsole.h>

#include <kernel/ports.h>
 
int kputchar(int ic) {
	char c = (char) ic;
	// outb(0x3F8, c);
	VirtualConsole::currentConsole->putchar(c);
	return ic;
}

int soutchar(int ic) {
	char c = (char) ic;
	outb(0x3F8, c);
	return ic;
}