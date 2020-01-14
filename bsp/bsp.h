#ifndef BSP_H
#define BSP_H

// All external access to STM bsp functions
#include "usart.h"

#ifdef cplusplus
extern "C" {
#endif

/*

General

*/
void bspInit(void);
void bspToggleLED(void);

/*

Timer Functions
* Register timer

Mostly defined in tim.c

*/
typedef void (*sysTickCallback_t)(void);

extern sysTickCallback_t sysTickCallbackPointer;
void bspRegisterSysTickCallback(sysTickCallback_t cbFn);

/*

Serial

*/

extern UartInstance_t VCP_UART;
extern UartInstance_t D01_UART;

#ifdef cplusplus
}
#endif

#endif
