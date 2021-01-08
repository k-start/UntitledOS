#!/bin/sh
set -e
. ./build.sh

if grep -q Microsoft /proc/version; then
  cmd.exe /c 'E:\wsl\installed\ubuntu-20.04\rootfs\home\kizzington\UntitledOS\windows.bat'
  # qemu-system-$(./target-triplet-to-arch.sh $HOST).exe -kernel kernel/untitledos.kernel -serial stdio -no-reboot -monitor stdio -no-shutdown
  # qemu-system-$(./target-triplet-to-arch.sh $HOST).exe -kernel kernel/untitledos.kernel -serial stdio -no-reboot
else
  qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel kernel/untitledos.kernel -serial stdio -no-reboot
fi
