#!/bin/bash
qemu-system-x86_64 -d int -hda Image/x64BareBonesImage.qcow2 -m 512 -rtc base=localtime 
