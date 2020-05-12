#include <kernel/kstdio.h>
#include <kernel/VirtualConsole.h>
 
int kputchar(int ic) {
	char c = (char) ic;
	VirtualConsole::currentConsole->putchar(c);
	return ic;
}