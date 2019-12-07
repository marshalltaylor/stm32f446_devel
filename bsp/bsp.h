#ifndef BSP_H
#define BSP_H

// All external access to STM bsp functions
#include "usart.h"

#ifdef cplusplus
extern "C" {
#endif
	
/* Callback typedefs */
typedef void (*sysTickCallback_t)(void);

extern UartInstance_t VCP_UART;
extern UartInstance_t D01_UART;
extern sysTickCallback_t sysTickCallbackPointer;

/* Function prototypes */
void bspInit(void);
void bspToggleLED(void);
void bspRegisterSysTickCallback(sysTickCallback_t cbFn);

#ifdef cplusplus
}
#endif

#endif