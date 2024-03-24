#!/bin/bash


if [[ $(lsmod | grep Module) == *"eeveevfs"* ]]; then
    echo "Shutting down eeveevfs.ko"
    sudo rmmod eeveevfs.ko
else
    echo "Loading up eeveevfs.ko"
    sudo insmod eeveevfs.ko
fi
