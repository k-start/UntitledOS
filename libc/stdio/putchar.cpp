#include <stdio.h>
 
#if defined(__is_libk)
#include <kernel/VirtualConsole.h>
#endif
 
int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	VirtualConsole::currentConsole->putchar(c);
#else
	// TODO: Implement stdio and the write system call.
#endif
	return ic;
}