#!/bin/sh
set -e
. ./build.sh

if grep -q Microsoft /proc/version; then
  qemu-system-$(./target-triplet-to-arch.sh $HOST).exe -kernel kernel/untitledos.kernel
else
  qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel kernel/untitledos.kernel
fi
