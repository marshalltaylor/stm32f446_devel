#ifndef BSP_H
#define BSP_H

// This file is an interface and should not include other files
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* General */
void bspInit(void);

/* IO */
#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define D8 8
#define D9 9
#define D10 10
#define D11 11
#define D12 12
#define D13 13
#define D14 14
#define D15 15
#define D16 16
#define D17 17
#define D18 18
#define D19 19
#define D20 20
#define D21 21
#define D22 22
#define D23 23
#define D24 24
#define D25 25
#define D26 26
#define D27 27
#define D28 28
#define D29 29
#define D30 30
#define D31 31
#define D32 32
#define D33 33
#define D34 34
#define D35 35
#define D36 36

#define A0 D16
#define A1 D17
#define A2 D18
#define A3 D19
#define A4 D20
#define A5 D21

//TODO: Used for init function but not tested
#define BSP_IO_OUTPUT 1
#define BSP_IO_INPUT 2
#define BSP_IO_INPUT_PULLUP 2
#define BSP_IO_INPUT_PULLDOWN 3

void bspIOToggleLED(void);

void bspIOPinInit(uint8_t pin, uint8_t pullMode);
void bspIOPinWrite(uint8_t pin, bool value);
bool bspIOPinRead(uint8_t pin);
uint32_t bspIOPinReadAnalog(uint8_t pin);

//Dirty api.  This may be run from within the bsp or by an app...
void bspADCConvert(void);

/* Spi operations */
void bspSPISegmentWrite(uint8_t *);
void bspSPISegmentSend(void);

/*

Timer Functions
* Register timer

Mostly defined in tim.c

*/
uint32_t millis(void);
uint32_t ulGetFastRunTimeTicks(void);

typedef void (*bspTimerCallback_t)(void);

extern bspTimerCallback_t timer3TickCallback;
extern bspTimerCallback_t sysTickCallbackPointer;

void bspRegisterSysTickCallback(bspTimerCallback_t cbFn);

/* Serial */
#define MAX_PRINTF_LEN 256
void bspSerialConsolePrintf(const char* fmt, ...);
uint8_t bspSerialConsolePeek(void);
void bspSerialConsoleWrite(uint8_t data);
uint8_t bspSerialConsoleRead(void);
uint16_t bspSerialConsoleBytesAvailable(void);

#ifdef __cplusplus
}
#endif

#endif
