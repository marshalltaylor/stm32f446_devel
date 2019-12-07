/* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include "stm32f4xx_hal.h"
//#include "adc.h"
//#include "dma.h"
//#include "spi.h"
//#include "tim.h"
//#include "usart.h"
//#include "gpio.h"

#include <stdint.h>
#include <stdbool.h>
#include "App.h"
#include "bsp.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private function prototypes -----------------------------------------------*/

int main(void)
{
	bspInit();
	
	AppEntry();
	
	while (1)
	{
	}
}
