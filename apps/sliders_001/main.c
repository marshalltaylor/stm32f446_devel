#include <stdint.h>
#include <stdbool.h>
#include "bsp.h"
#include "main.h"
#include "sketch.h"

/* Private variables ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private function prototypes -----------------------------------------------*/

int main(void)
{
	bspInit();
	
	setup();
	
	while(1)
	{
		loop();
	}
}
