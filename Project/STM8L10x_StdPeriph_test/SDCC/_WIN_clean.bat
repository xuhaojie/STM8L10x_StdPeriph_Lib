REM just for output
echo off
cls

REM target device (doesn't matter here)
set DEVICE=STM8L10x

REM set make tool (if not in PATH, set complete path)
set MAKE=mingw32-make

REM use makefile to delete sdcc output
%MAKE% -f Makefile DEVICE=%DEVICE% clean

echo on

