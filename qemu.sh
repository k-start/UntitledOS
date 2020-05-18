#!/bin/sh
set -e
. ./iso.sh
 
 
if grep -q Microsoft /proc/version; then
  qemu-system-$(./target-triplet-to-arch.sh $HOST).exe -boot d -cdrom untitledos.iso -m 512 -hda mydisk.img
else
  qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom untitledos.iso
fi