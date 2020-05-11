#ifndef TASKCRT_H
#define TASKCRT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CRTVideo.h"

extern CRTVideo crt;

void taskCRTStart(void * argument);

	
#ifdef __cplusplus
}
#endif

#endif
