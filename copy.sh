#!/bin/bash

DEVICE=/dev/sdb
PART="${DEVICE}1"

FILE=build/hello_world.uf2


if [ -z $1 ]; then
    echo "Not provided: $1"
    FILE=build/hello_world.uf2
else
    echo "Provided: $1"
    FILE=$1
fi;

fdisk -l $DEVICE | grep "model: RP2"

DEVICE_EXISTS=$?

if [ $DEVICE_EXISTS != 0 ]; then
    echo "Device not found. Aborting.";
    exit 1;
fi

mount $PART /mnt/rpi

cmake --build build/ -j`nproc`

if [ -f $FILE ]; then
    cp $FILE /mnt/rpi;
    sync
    umount /mnt/rpi
else
    echo "No file $FILE"
    umount /mnt/rpi
    exit 1;
fi
