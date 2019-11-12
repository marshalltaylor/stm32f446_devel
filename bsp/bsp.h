#ifndef BSP_H
#define BSP_H

// All external access to STM bsp functions
#include "usart.h"

extern UartInstance_t VCP_UART;
extern UartInstance_t D01_UART;

// Local functions
void bspInit(void);
void bspToggleLED(void);

#endif