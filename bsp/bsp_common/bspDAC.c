/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "dac.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"

/* References ----------------------------------------------------------------*/
typedef enum
{
	CRT_STATE_SYNC_TIP = 0;
	CRT_STATE_BACK_PORCH,
	CRT_STATE_VIDEO,
	CRT_STATE_FRONT_PORCH
} crtStates_t;

#define CRT_PARAM_SYNC_TIP_LEN 6
#define CRT_PARAM_BACK_PORCH_LEN 44
#define CRT_PARAM_VIDEO_LEN 50
#define CRT_PARAM_FRONT_PORCH_LEN 25

#define CRT_PARAM_SYNC_TIP_LEVEL 0x30
#define CRT_PARAM_PORCH_LEVEL 0x50

#define GENERIC_BUFFER_LEN 256
/* ... video buffers ... */
uint8_t syncTip[GENERIC_BUFFER_LEN];
uint8_t porch[GENERIC_BUFFER_LEN];
//uint8_t video[640][480]; // = 307.2 kB
//uint8_t video[320][240]; // = 76.8 kB
uint8_t testVideo[GENERIC_BUFFER_LEN];

/* Functions -----------------------------------------------------------------*/
void bspDACSendBuffer(uint32_t * buffer, uint16_t len)
{
	if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, buffer, len, DAC_ALIGN_8B_R) != HAL_OK)
	{
		volatile char garbage = 2;
		UNUSED(garbage);
	}
	
	//HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, 0x00000044);
}

static void bspDACStartDMA(void)
{
	

}

/* ... state variables ... */
crtStates_t crtState = CRT_STATE_INIT;
uint16_t lineNumber = 0;
uint8_t * pVideoPage = NULL;

void bspDACInit( void )
{
	crtState = CRT_STATE_SYNC_TIP;
	int i;
	for(i = 0; i < GENERIC_BUFFER_LEN; i++)
	{
		syncTip[i] = CRT_PARAM_SYNC_TIP_LEVEL;
		porch[i] = CRT_PARAM_PORCH_LEVEL;
		testVideo[i] = 0x80;
	}
	bspDACStartDMA();
}

bool bspDACPopState( uint8_t ** ppDataAdr, uint8_t len )
{
	//uint8_t actualData[];
	//uint8_t * pData = actualData;
	//*ppDataAdr = pData;
	
	switch( crtState )
	{
		case CRT_STATE_SYNC_TIP:
		{
			*ppDataAdr = syncTip;
			*len = CRT_PARAM_SYNC_TIP_LEN;
			
			crtState = CRT_STATE_BACK_PORCH;
			break;
		}
		case CRT_STATE_BACK_PORCH:
		{
			*ppDataAdr = porch;
			*len = CRT_PARAM_BACK_PORCH_LEN;
			
			crtState = CRT_STATE_VIDEO;
			break;
		}
		case CRT_STATE_VIDEO:
		{
			if( pVideoPage == NULL )
			{
				*ppDataAdr = testVideo;
			}
			else
			{
				//*ppDataAdr = page4 or whatever
			}
			*len = CRT_PARAM_VIDEO_LEN;
			
			crtState = CRT_STATE_FRONT_PORCH;
			break;
		}
		case CRT_STATE_FRONT_PORCH:
		{
			*ppDataAdr = porch;
			*len = CRT_PARAM_FRONT_PORCH_LEN;
			
			crtState = CRT_STATE_SYNC_TIP;
			break;
		}
		default:
	}
	return true;
}





