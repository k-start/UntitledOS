DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)
 
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc
 
ARCHDIR=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
KERNEL_OBJS=\
$(ARCHDIR)/boot.o \
$(ARCHDIR)/gdtAsm.o \
$(ARCHDIR)/gdt.o \
$(ARCHDIR)/tty.o \
$(ARCHDIR)/interrupts.o \
$(ARCHDIR)/ports.o \
$(ARCHDIR)/CPU.o \
$(ARCHDIR)/VirtualConsole.o \
kernel/kernel.o \
devices/KeyboardDevice.o \
IRQHandler.o \
PIT.o \
RTC.o \
kprintf.o \
kputchar.o \
 
OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
 
LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
 
.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c++ .S
 
all: untitledos.kernel
 
untitledos.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot untitledos.kernel
 
$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@
 
.c++.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
 
.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.asm
	nasm $< -f elf -o $@
 
clean:
	rm -f untitledos.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.
 
install-kernel: untitledos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp untitledos.kernel $(DESTDIR)$(BOOTDIR)
 
-include $(OBJS:.o=.d)