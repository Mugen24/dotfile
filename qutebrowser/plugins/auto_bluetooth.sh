#!/bin/bash
if [[ $(bluetoothctl show | grep Powered |cut -d " " -f 2) == yes ]]
then
    bluetoothctl power off
else
    bluetoothctl power on
    for device in "$(bluetoothctl devices)"
    do
        mac=$(echo $device | cut -d " " -f 2)
        bluetoothctl connect $mac
    done
fi

