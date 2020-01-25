#ifndef BSP_H
#define BSP_H

// This file is an interface and should not include other files
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* General */
void bspInit(void);

/* IO */
void bspIOToggleLED(void);
void bspADCConvert(void);
//extern uint16_t adcValues[10];

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
