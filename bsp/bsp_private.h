#ifndef BSP_PRIVATE_H
#define BSP_PRIVATE_H

#ifdef cplusplus
extern "C" {
#endif

#include "bsp.h"

//void (*timer3TickCallback)( void );
extern bspTimerCallback_t timer3TickCallback;
extern bspTimerCallback_t sysTickCallbackPointer;

//Used for uart monitoring right now
//extern bspTimerCallback_t timer4TickCallback;
#ifdef cplusplus
}
#endif

#endif
