#!/bin/sh
set -e
. ./iso.sh
 
 
if grep -q Microsoft /proc/version; then
  qemu-system-$(./target-triplet-to-arch.sh $HOST).exe -cdrom untitledos.iso
else
  qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom untitledos.iso
fi