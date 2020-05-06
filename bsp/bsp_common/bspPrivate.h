#ifndef BSP_PRIVATE_H
#define BSP_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"

void bspPwmSampleIsr(void);

//Used for uart monitoring right now
//extern bspTimerCallback_t timer4TickCallback;
#ifdef __cplusplus
}
#endif

#endif
