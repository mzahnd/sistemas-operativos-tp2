#!/bin/bash
readonly SCRIPT_DIR="$(dirname ${BASH_SOURCE[0]})"
pushd "$SCRIPT_DIR" &> /dev/null

if [[ "$1" = "gdb" ]]; then
    qemu-system-x86_64 -s -S -d int -hda Image/x64BareBonesImage.qcow2 -m 512 -rtc base=localtime
else
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -rtc base=localtime 
fi

popd &> /dev/null # $SCRIPT_DIR
