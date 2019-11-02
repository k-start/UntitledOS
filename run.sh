#!/bin/sh
set -e
. ./build.sh

qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel kernel/untitledos.kernel