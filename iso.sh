#!/bin/sh
set -e
. ./build.sh
 
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
 
cp sysroot/boot/untitledos.kernel isodir/boot/untitledos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "untitledos" {
	multiboot /boot/untitledos.kernel
}
EOF
grub-mkrescue -o untitledos.iso isodir