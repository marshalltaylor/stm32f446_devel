/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"

/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void crapDelay( int16_t seconds );

int main(void)
{
	bspInit();
	
	// Run tests here
	
	//AppEntry();
	while (1)
	{
		bspToggleLED();
		crapDelay(1);
	}
}

void crapDelay( int16_t seconds )
{
	volatile int32_t startTime;
	//startTime = millis();
	//while(millis() < startTime + 1000);
	int32_t i;
	int32_t j;
	for(i = 0; i < 3000; i++)
	{
		for(j = 0; j < 1000; j++)
		{
			startTime++;
		}
	}
	
}
