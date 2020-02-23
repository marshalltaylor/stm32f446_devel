/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Includes -- BSP -----------------------------------------------------------*/
//#include "bsp.h"
#include "bspPins.h"

#ifdef __cplusplus
extern "C" {
#endif
//int  var[3] = {10, 100, 200};
// Pin number
//char * sdfsdfsdDToPort[4] = {
////	GPIOJ,
////	GPIOJ
//};

//usage: ex: GPIOD expansion:
//(GPIO_TypeDef *) (AHB1PERIPH_BASE + DxToPort[3])

const portPinOffsets_t DxToPortPin[] = {
	{ PORTA_OFFSET, GPIO_PIN_3  },  //D0 huart2
	{ PORTA_OFFSET, GPIO_PIN_2  },  //D1 huart2
	{ PORTA_OFFSET, GPIO_PIN_10 },  //D2
	{ PORTB_OFFSET, GPIO_PIN_3  },  //D3
	{ PORTB_OFFSET, GPIO_PIN_5  },  //D4
	{ PORTB_OFFSET, GPIO_PIN_4  },  //D5
	{ PORTB_OFFSET, GPIO_PIN_10 },  //D6
	{ PORTA_OFFSET, GPIO_PIN_8  },  //D7
	{ PORTA_OFFSET, GPIO_PIN_9  },  //D8
	{ PORTC_OFFSET, GPIO_PIN_7  },  //D9 ST-Link Tx (VCP to MCU) huart6
	{ PORTB_OFFSET, GPIO_PIN_6  },  //D10
	{ PORTA_OFFSET, GPIO_PIN_7  },  //D11
	{ PORTA_OFFSET, GPIO_PIN_6  },  //D12
	{ PORTA_OFFSET, GPIO_PIN_5  },  //D13 User LED
	{ PORTB_OFFSET, GPIO_PIN_9  },  //D14 SDA
	{ PORTB_OFFSET, GPIO_PIN_8  },  //D15 SCL
	{ PORTA_OFFSET, GPIO_PIN_0  },  //D16/A0
	{ PORTA_OFFSET, GPIO_PIN_1  },  //D17/A1
	{ PORTA_OFFSET, GPIO_PIN_4  },  //D18/A2
	{ PORTB_OFFSET, GPIO_PIN_0  },  //D19/A3
	{ PORTC_OFFSET, GPIO_PIN_1  },  //D20/A4
	{ PORTC_OFFSET, GPIO_PIN_0  },  //D21/A5
	{ PORTC_OFFSET, GPIO_PIN_13 },  //D22 User btn
	{ PORTC_OFFSET, GPIO_PIN_6  },  //D23 ST-Link Rx (MCU to VCP) huart6
	{ PORTC_OFFSET, GPIO_PIN_8  },  //D24 in
	{ PORTC_OFFSET, GPIO_PIN_5  },  //D25 in
	{ PORTA_OFFSET, GPIO_PIN_12 },  //D26 in
	{ PORTA_OFFSET, GPIO_PIN_11 },  //D27 in
	{ PORTB_OFFSET, GPIO_PIN_12 },  //D28 in
	{ PORTB_OFFSET, GPIO_PIN_2  },  //D29 in
	{ PORTB_OFFSET, GPIO_PIN_1  },  //D30 in
	{ PORTB_OFFSET, GPIO_PIN_15 },  //D31 in
	{ PORTB_OFFSET, GPIO_PIN_14 },  //D32 in
	{ PORTB_OFFSET, GPIO_PIN_13 }   //D33 in
};

#ifdef __cplusplus
}
#endif
