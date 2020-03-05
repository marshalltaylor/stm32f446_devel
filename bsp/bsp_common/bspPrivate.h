#ifndef BSP_PRIVATE_H
#define BSP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

//Used for uart monitoring right now
//extern bspTimerCallback_t timer4TickCallback;

bool bspDACPopState( uint8_t ** ppDataAdr, uint8_t len );

#ifdef __cplusplus
}
#endif

#endif
