#!/bin/bash
if [ $# -eq 0 ]; then
    echo "Must pass Local IP as Argument"
    exit
fi

MY_IP="$1"

if [ -f /.dockerenv ]; then
    echo "Debugging inside docker";
else
    echo "Debugging on host";
    sudo rm -rf /itba/*
fi

echo $HOME

cat .gdbinit > /home/pedro/.gdbinit

sed -i "1 s/.*/target remote ${MY_IP}:1234/" ${HOME}/.gdbinit

sudo cp -r -f ${PWD}/* /itba/
gdb