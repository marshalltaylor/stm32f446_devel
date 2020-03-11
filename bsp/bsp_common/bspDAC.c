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
	CRT_STATE_SYNC_TIP = 0,
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

static bool bspDACPopState( uint8_t ** ppData );

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

void bspDACStartDMA(void)
{
	uint8_t * pData = NULL;
	uint8_t * pDataM1 = NULL;
	bspDACPopState( &pData );
	bspDACPopState( &pDataM1 );
	//if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)pData, len, DAC_ALIGN_8B_R) != HAL_OK)
	//if(HAL_DAC_Test_Ping_Pong(&hdac, DAC_CHANNEL_1))
	if(HAL_DAC_START_DOUBLE(&hdac, DAC_CHANNEL_1, (uint32_t*)pData, (uint32_t*)pDataM1, 16))
	{
		volatile char garbage = 2;
		UNUSED(garbage);
	}
}
static uint8_t testDACBuffer [] = {
	255, 247, 239, 231, 223, 215, 207, 199, 191, 183, 175, 167, 159, 151, 143, 135,
	127, 119, 111, 103, 95, 87, 79, 71, 63, 55, 47, 255, 255, 255, 0, 0,
	0, 255, 255, 255, 47, 55, 63, 71, 79, 87, 95, 103, 111, 119, 127, 135,
	143, 151, 159, 167, 175, 183, 191, 199, 207, 215, 223, 231, 239, 247, 255, 0 
};
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	uint8_t * pData = NULL;
	bspDACPopState( &pData );
	
	hdac->DMA_Handle1->Instance->M0AR = (uint32_t)pData;
}

void HAL_DAC_M1ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	uint8_t * pData = NULL;
	bspDACPopState( &pData );
	
	hdac->DMA_Handle1->Instance->M1AR = (uint32_t)pData;
}

/* ... state variables ... */
crtStates_t crtState = CRT_STATE_SYNC_TIP;
uint16_t lineNumber = 0;
uint8_t * pVideoPage = NULL;

void bspDACInit( void )
{
	UNUSED(testDACBuffer);
	
	crtState = CRT_STATE_SYNC_TIP;
	int i;
	for(i = 0; i < GENERIC_BUFFER_LEN; i++)
	{
		syncTip[i] = CRT_PARAM_SYNC_TIP_LEVEL;
		porch[i] = CRT_PARAM_PORCH_LEVEL;
		testVideo[i] = 0xFF & (i*10);
	}
	bspDACStartDMA();
}

static bool bspDACPopState( uint8_t ** ppData )
{
	switch( crtState )
	{
		case CRT_STATE_SYNC_TIP:
		{
			bspIOPinWrite(D31, 0);
			bspIOPinWrite(D31, 1);
			*ppData = syncTip;

			crtState = CRT_STATE_BACK_PORCH;
			break;
		}
		case CRT_STATE_BACK_PORCH:
		{
			*ppData = porch;
			
			crtState = CRT_STATE_VIDEO;
			break;
		}
		case CRT_STATE_VIDEO:
		{
			if( pVideoPage == NULL )
			{
				*ppData = testVideo;
			}
			else
			{
				//*ppData = page4 or whatever
			}
			crtState = CRT_STATE_FRONT_PORCH;
			break;
		}
		case CRT_STATE_FRONT_PORCH:
		{
			*ppData = porch;
			
			crtState = CRT_STATE_SYNC_TIP;
			break;
		}
		default:
		break;
	}
	return true;
}





