/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "gpio.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"
#include "bspPins.h"

/* References ----------------------------------------------------------------*/
static bool ledState = false;
extern uint16_t adcValues[];

/* Functions -----------------------------------------------------------------*/
void bspIOToggleLED(void)
{
	ledState =! ledState;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, ledState);
}

// defaulted in bsp, this can be used to override pulls
void bspIOPinInit(uint8_t pin, uint8_t pullMode)
{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	
//	GPIO_InitStruct.Pin = DxToPortPin[pin].pinOffset;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init((GPIO_TypeDef *) (AHB1PERIPH_BASE + DxToPortPin[pin]), &GPIO_InitStruct);
}

void bspIOPinWrite(uint8_t pin, bool value)
{
	GPIO_PinState bspValue = GPIO_PIN_RESET;
	if( value ) bspValue = GPIO_PIN_SET;
	HAL_GPIO_WritePin((GPIO_TypeDef *) (AHB1PERIPH_BASE + DxToPortPin[pin].portOffset), DxToPortPin[pin].pinOffset, bspValue);
}

bool bspIOPinRead(uint8_t pin)
{
	if( HAL_GPIO_ReadPin((GPIO_TypeDef *) (AHB1PERIPH_BASE + DxToPortPin[pin].portOffset), DxToPortPin[pin].pinOffset) == GPIO_PIN_SET )
	{
		return true;
	}
	return false;
}

//perform the read operation on the selected analog pin.
uint32_t bspIOPinReadAnalog(uint8_t pin)
{
	uint32_t value = 0;
	if(pin == A0) value = adcValues[0];
	if(pin == A1) value = adcValues[1];
	if(pin == A2) value = adcValues[2];
	if(pin == A3) value = adcValues[3];
	if(pin == A4) value = adcValues[4];
	if(pin == A5) value = adcValues[5];
	return value;
}