#!/bin/bash

DEVICE=/dev/sda
PART="${DEVICE}1"

FILE=build/hello_world.uf2


if [ -z $1 ]; then
    FILE=build/clock.uf2
    echo "Missing argument. Defaulting to copying $FILE"
else
    echo "Provided: $1"
    FILE=$1
fi;

echo "Checking if device is plugged in"
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
