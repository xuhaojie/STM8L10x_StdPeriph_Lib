REM just for output
echo off
cls

REM target device (doesn't matter here)
set DEVICE=STM8L10x

REM set make tool (if not in PATH, set complete path)
set MAKE=mingw32-make

REM STM8L10x has no bootloader, see tech note UM0560 and TN0189 on http://www.st.com

REM set SWIM upload tool and device name (stm8flash from https://github.com/vdudouyt/stm8flash)
REM set SWIM_LOADER=C:\Programme\stm8flash\stm8flash.exe"
REM set SWIM_TOOL=stlink
REM set SWIM_DEVICE=stm8l101?3

REM set SWIM upload tool and device name (STVP-STM8 on http://www.st.com)
set SWIM_LOADER="C:\Programme\STMicroelectronics\st_toolset\stvp\STVP_CmdLine.exe"
set SWIM_DEVICE=STM8L101x3

REM name of target hexfile
set TARGET=.\%DEVICE%\%DEVICE%.hex

REM compile and link
echo make application
%MAKE% -f Makefile DEVICE=%DEVICE%
IF ERRORLEVEL 1 GOTO END
echo done with application
echo.

REM GOTO END

REM upload using SWIM debug interface (stm8flash from https://github.com/vdudouyt/stm8flash)
REM %SWIM_LOADER% -c %SWIM_TOOL% -w %TARGET% -p %SWIM_DEVICE%

REM upload using SWIM debug interface (STVP-STM8 on http://www.st.com)
%SWIM_LOADER% -BoardName=ST-LINK -Port=USB -ProgMode=SWIM -Device=%DEVICE_STVP% -readData -readOption -no_progData -no_progOption -no_loop -no_log -FileProg=%TARGET%

REM clean up
:END
PAUSE
echo on

