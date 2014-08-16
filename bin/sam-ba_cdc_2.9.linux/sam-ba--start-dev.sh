#/bin/bash
sudo modprobe usbserial vendor=0x03eb product=0x6124  
sudo modprobe g_cdc
sudo modprobe cdc-acm
sudo chmod 666 /dev/ttyUSB0
ls -alF --color /dev/ttyUSB0
