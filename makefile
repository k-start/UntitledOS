CC=i686-elf-g++
CFLAGS=-ffreestanding
C_SOURCES = $(wildcard kernel/*.c++)
HEADERS = $(wildcard kernel/*.h)
OBJ = ${C_SOURCES:.c++=.o}

%.o: %.c++ ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

run: os.bin
	qemu-system-i386 -kernel os.bin

boot.o:
	i686-elf-as boot/boot.s -o boot.o

os.bin: boot.o ${OBJ}
	${CC} -T linker.ld -o os.bin -ffreestanding -O2 -nostdlib $^ -lgcc

clean:
	rm -rf *.bin *.dis *.o os.bin *.elf
	rm -rf kernel/*.o