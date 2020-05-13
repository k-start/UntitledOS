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
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib
 
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_liby -Iinclude
LIBKY_CFLAGS:=$(CFLAGS)
LIBKY_CPPFLAGS:=$(CPPFLAGS) -D__is_libky
 
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBKY_CFLAGS:=$(LIBKY_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBKY_CPPFLAGS:=$(LIBKY_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
 
FREEOBJS=\
$(ARCH_FREEOBJS) \
Vector.o \
 
HOSTEDOBJS=\
$(ARCH_HOSTEDOBJS) \
 
OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \
 
LIBKY_OBJS=$(FREEOBJS:.o=.libky.o)

BINARIES=libky.a
 
.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libky.o .cpp .S
 
all: $(BINARIES)
 
liby.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
 
libky.a: $(LIBKY_OBJS)
	$(AR) rcs $@ $(LIBKY_OBJS)
 
.cpp.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
 
.cpp.S:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)
 
.cpp.libky.o:
	$(CC) -MD -c $< -o $@ $(LIBKY_CFLAGS) $(LIBKY_CPPFLAGS)
 
.S.libky.o:
	$(CC) -MD -c $< -o $@ $(LIBKY_CFLAGS) $(LIBKY_CPPFLAGS)
 
clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBKY_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBKY_OBJS:.o=.d) *.d */*.d */*/*.d
 
install: install-headers install-libs
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)/Y
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/Y/.
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
 
-include $(OBJS:.o=.d)
-include $(LIBKY_OBJS:.o=.d)