# MIDI clock
The midi clock is a device that can accept and generate midi clock messages.  The hardware is a STM32F466 dev board with midi shield and controls.

## Status
Currently in massive rework -- check earlier commits for old working version

By layer,
* apps -- production and test apps exist
* FreeRTOS -- extracted from cubemx and compiled as archive.  Works for test app
* drivers -- seven segment display written as test driver, perhaps the name is wrong.  This layer needs work
* arduino -- should probably be abandoned or turned into driver pieces
* bsp -- wraps STM project and provides sole point of entry for the outer world.  Coming along nicely

## Commands
`make` -- build the application

`make info` -- print object and include lists for debugging

`make openocd` -- Start the openocd server to maintain a link with the hw.  This usually doesn't need to be restarted

`make deploy` -- Start the debugging session.  Close the resultant window to end the session.

Debugging utalizes a gdb session connected to an openocd server.  Example commands that could be run manually are shown to illustrate what the system is doing.
```
C:\Users\marshall>openocd -f "E:\github\something\something\usbDebug.cfg" -c "init; reset halt"

C:/STM32/gcc-arm-none-eabi/bin/arm-none-eabi-gdb -x gdbinit.txt build/output.elf
```

## Hardware
Reroute HID serial to uart 6. 115200 baud
* Open jumpers SB13 and SB14
* Close jumpers SB62 and SB63
* Link CN3, RX to CN10, pin 4
* Link CN3, TX to CN10, pin 19

Uart 2 is midi. 31500 baud
*Arduino pins D0/1
