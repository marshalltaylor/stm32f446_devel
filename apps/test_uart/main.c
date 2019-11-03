/* Includes ------------------------------------------------------------------*/
#include "main_bsp.h"

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void crapDelay( int16_t seconds );

int main(void)
{
	main_bsp();
	
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
	// Run tests here
	
	//AppEntry();
	while (1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		crapDelay(1);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
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
