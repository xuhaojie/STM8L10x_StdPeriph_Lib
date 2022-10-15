#!/bin/bash 

# open terminal on double-click, skip else. See https://www.linuxquestions.org/questions/programming-9/executing-shell-script-in-terminal-directly-with-a-double-click-370091/
tty -s; if [ $? -ne 0 ]; then /etc/alternatives/x-terminal-emulator -e "$0"; exit; fi

# change to current working directory
cd `dirname $0`

# just for output
clear

# target device (doesn't matter here)
DEVICE=STM8L10x

# set make tool (if not in PATH, set complete path)
MAKE=make

# STM8L10x has no bootloader, see tech note UM0560 and TN0189 on http://www.st.com

# set SWIM upload tool and device name (stm8flash from https://github.com/vdudouyt/stm8flash)
SWIM_LOADER=stm8flash
SWIM_TOOL=stlink
SWIM_DEVICE=stm8l101?3

# target hexfile
TARGET=./$DEVICE/$DEVICE.hex

# make project
echo "make application"
$MAKE -f Makefile DEVICE=$DEVICE
if [ $? -ne 0 ]; then
  echo " "
  read -p "press key to close window..."
  echo on
  exit
fi
echo "done with application"
echo " "

# upload using SWIM debug interface
$SWIM_LOADER -c $SWIM_TOOL -w $TARGET -p $SWIM_DEVICE

echo " "
read -p "press key to close window..."
echo " "


