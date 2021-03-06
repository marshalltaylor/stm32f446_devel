/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

#include "gpio.h"
#include "dma.h"

/* USER CODE BEGIN 0 */
#include "custom_stm32f4xx_hal_uart.h"
#include "debugUtilities.h" // traceWrite(NAVY, 1);
#include "tim.h"
UartInstance_t VCP_UART;
UartInstance_t D01_UART;

bool uartNeedsRestarted[UARTS_MONITORED] = {0};

/* USER CODE END 0 */

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart6_tx;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{
  D01_UART.huart = &huart2;
//  D01_UART.hdma_rx = &hdma_usart2_rx;
  D01_UART.hdma_tx = &hdma_usart2_tx;
  
  D01_UART.txDataBuffer_head = 0;
  D01_UART.txDataBuffer_next = 0;
  D01_UART.txDataBuffer_tail = 0;

  D01_UART.rxDataBuffer_first = 0;
  D01_UART.rxDataBuffer_last = 0;
  D01_UART.rxDataBuffer[0] = 0;
  
  D01_UART.UartTxInProgress = false;
  
  D01_UART.huart->Instance = USART2;
  D01_UART.huart->Init.BaudRate = 31250;
  D01_UART.huart->Init.WordLength = UART_WORDLENGTH_8B;
  D01_UART.huart->Init.StopBits = UART_STOPBITS_1;
  D01_UART.huart->Init.Parity = UART_PARITY_NONE;
  D01_UART.huart->Init.Mode = UART_MODE_TX_RX;
  D01_UART.huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  D01_UART.huart->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(D01_UART.huart) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  HAL_UART_Receive_IT(D01_UART.huart, (uint8_t *)&D01_UART.rxCharBuffer, 1);
  //__HAL_UART_ENABLE_IT(D01_UART.huart, UART_IT_RXNE);

}
/* USART6 init function */

void MX_USART6_UART_Init(void)
{
  VCP_UART.huart = &huart6;
//  VCP_UART.hdma_rx = &hdma_usart6_rx;
  VCP_UART.hdma_tx = &hdma_usart6_tx;
  
  VCP_UART.txDataBuffer_head = 0;
  VCP_UART.txDataBuffer_next = 0;
  VCP_UART.txDataBuffer_tail = 0;

  VCP_UART.rxDataBuffer_first = 0;
  VCP_UART.rxDataBuffer_last = 0;
  VCP_UART.rxDataBuffer[0] = 0;
  
  VCP_UART.UartTxInProgress = false;
  
  VCP_UART.huart->Instance = USART6;
  VCP_UART.huart->Init.BaudRate = 115200;
  VCP_UART.huart->Init.WordLength = UART_WORDLENGTH_8B;
  VCP_UART.huart->Init.StopBits = UART_STOPBITS_1;
  VCP_UART.huart->Init.Parity = UART_PARITY_NONE;
  VCP_UART.huart->Init.Mode = UART_MODE_TX_RX;
  VCP_UART.huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  VCP_UART.huart->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(VCP_UART.huart) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  HAL_UART_Receive_IT(VCP_UART.huart, (uint8_t *)&VCP_UART.rxCharBuffer, 1);
  //__HAL_UART_ENABLE_IT(VCP_UART.huart, UART_IT_RXNE);
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_TX Init */
    hdma_usart2_tx.Instance = DMA1_Stream6;
    hdma_usart2_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();
  
    /**USART6 GPIO Configuration    
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART6 DMA Init */
    /* USART6_TX Init */
    hdma_usart6_tx.Instance = DMA2_Stream6;
    hdma_usart6_tx.Init.Channel = DMA_CHANNEL_5;
    hdma_usart6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart6_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart6_tx.Init.Mode = DMA_NORMAL;
    hdma_usart6_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart6_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart6_tx) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart6_tx);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, USART_TX_Pin|USART_RX_Pin);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();
  
    /**USART6 GPIO Configuration    
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    /* USART6 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
static void uartQueueNextData(UartInstance_t * UART)
{
	int16_t length;
	if(UART->UartTxInProgress == false)
	{
		length = UART->txDataBuffer_tail - UART->txDataBuffer_head;
		if(length <= 0)
		{
			length = TX_BUFFER_SIZE - UART->txDataBuffer_head;
			UART->txDataBuffer_next = 0;
			UART->UartTxInProgress = true;
			HAL_UART_Transmit_DMA(UART->huart, (uint8_t *)(UART->txDataBuffer + UART->txDataBuffer_head), length);
		}
		else if(length > 0)
		{
			UART->txDataBuffer_next = UART->txDataBuffer_tail;
			UART->UartTxInProgress = true;
			HAL_UART_Transmit_DMA(UART->huart, (uint8_t *)(UART->txDataBuffer + UART->txDataBuffer_head), length);
		}			
	}
}

uint8_t halUartPeek(UartInstance_t * UART)
{
	return UART->rxDataBuffer[UART->rxDataBuffer_first];
}

void halUartWrite(uint8_t data, UartInstance_t * UART)
{
	while(UART->UartTxInProgress == true);
	UART->txDataBuffer[UART->txDataBuffer_tail] = data;
	UART->txDataBuffer_tail++;
	if( UART->txDataBuffer_tail >= TX_BUFFER_SIZE )
	{
		UART->txDataBuffer_tail = 0;
	}
	if( UART->txDataBuffer_tail == UART->txDataBuffer_head )
	{
		UART->txDataBuffer_tail--;
		if( UART->txDataBuffer_tail < 0 )
		{
			UART->txDataBuffer_tail = TX_BUFFER_SIZE - 1;
		}
	}
	uartQueueNextData(UART);
}

uint8_t halUartRead(UartInstance_t * UART)
{
	uint8_t c = halUartPeek(UART);
	if(UART->rxDataBuffer_first != UART->rxDataBuffer_last)
	{
		UART->rxDataBuffer_first++;
		if( UART->rxDataBuffer_first >= RX_BUFFER_SIZE )
		{
			UART->rxDataBuffer_first = 0;
		}
	}
	return c;
}

uint16_t halUartReadBytesAvailable(UartInstance_t * UART)
{
	uint16_t var = UART->rxDataBuffer_last - UART->rxDataBuffer_first;
	if( var < 0 )
	{
		var += RX_BUFFER_SIZE;
	}
	return var;
}
/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of DMA Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if(UartHandle->Instance==USART2)
  {
    /* Set transmission flag: trasfer complete*/
    D01_UART.UartTxInProgress = false;
    D01_UART.txDataBuffer_head = D01_UART.txDataBuffer_next;
    if(D01_UART.txDataBuffer_next != D01_UART.txDataBuffer_tail)
    {
	    uartQueueNextData(&D01_UART);
    }
  }
  else if(UartHandle->Instance==USART6)
  {
    /* Set transmission flag: trasfer complete*/
    VCP_UART.UartTxInProgress = false;
    VCP_UART.txDataBuffer_head = VCP_UART.txDataBuffer_next;
    if(VCP_UART.txDataBuffer_next != VCP_UART.txDataBuffer_tail)
    {
	    uartQueueNextData(&VCP_UART);
    }
  }
 
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
//DebugObject_t usrUartDB;
uint32_t restartedCount = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	//traceWrite(YELLOW, 1);
  if(UartHandle->Instance==USART2)
  {
	//traceWrite(CYAN, 1);
	D01_UART.rxDataBuffer[D01_UART.rxDataBuffer_last] = D01_UART.rxCharBuffer;
	D01_UART.rxDataBuffer_last++;
	if( D01_UART.rxDataBuffer_last >= RX_BUFFER_SIZE )
	{
		D01_UART.rxDataBuffer_last = 0;
	}
	//Queue another rx transfer
	uint32_t result = (uint32_t)HAL_UART_Receive_IT(&huart2, (uint8_t *)&D01_UART.rxCharBuffer, 1);
	if(result == 0x02)
	{
		restartedCount++;
		debugLogRecord(__LINE__, result, "restart");
		uartNeedsRestarted[2] = true;
		startTim4Isr();
	}
  }
  else if(UartHandle->Instance==USART6)
  {
	VCP_UART.rxDataBuffer[VCP_UART.rxDataBuffer_last] = VCP_UART.rxCharBuffer;
	//halUartWrite(VCP_UART.rxCharBuffer,&VCP_UART); //echo
	VCP_UART.rxDataBuffer_last++;
	if( VCP_UART.rxDataBuffer_last >= RX_BUFFER_SIZE )
	{
		VCP_UART.rxDataBuffer_last = 0;
	}
	//Queue another rx transfer
	uint32_t result = (uint32_t)HAL_UART_Receive_IT(&huart6, (uint8_t *)&VCP_UART.rxCharBuffer, 1);
	if(result == 0x02)
	{
		restartedCount++;
		uartNeedsRestarted[6] = true;
		startTim4Isr();
		debugLogRecord(__LINE__, restartedCount, "restart");
	}
  }  
  //TODO: Put restart code if status = 0x02
}

void emergencyRestart(void)
{
	HAL_UART_Receive_IT(&huart6, (uint8_t*)&VCP_UART.rxCharBuffer, 1);
	while(1);
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    //Error_Handler();
	if(UartHandle->ErrorCode == HAL_UART_ERROR_DMA)
	{
		//Whatev's, kick it with a fresh buffer
		if(UartHandle->Instance==USART2)
		{
			//while(1);
			//Queue another rx transfer
			//HAL_UART_Receive_DMA(&huart2, &D01_UART.rxCharBuffer, 1);
		}
		else if(UartHandle->Instance==USART6)
		{
			//while(1);
			//Queue another rx transfer
			//HAL_UART_Receive_DMA(&huart6, &VCP_UART.rxCharBuffer, 1);
		}
		UartHandle->ErrorCode &= ~HAL_UART_ERROR_DMA;
	}
	uartNeedsRestarted[6] = true;
	restartedCount++;
	startTim4Isr();
	debugLogRecord(__LINE__, (uint32_t)UartHandle->ErrorCode, "error");
}

//Right now, invoked from ISR
//Could be invoked on a schedule
void restartHuartIfNeeded(void)
{
	bool AnyRestartsNeeded = false;
	//debugLogRecord(__LINE__, 0xFF, "rsISR");
	for(int i = 0; i < UARTS_MONITORED; i++)
	{
		if(uartNeedsRestarted[i])
		{
			debugLogRecord(__LINE__, i, "handle");
			uartNeedsRestarted[i] = false; // may be overridden
			switch( i )
			{
				case 2:
				{
					uint32_t result = (uint32_t)HAL_UART_Receive_IT(&huart2, (uint8_t *)&D01_UART.rxCharBuffer, 1);
					if(result != HAL_OK)
					{
						uartNeedsRestarted[i] = true;
						AnyRestartsNeeded = true;
					}
				}
				break;
				case 6:
				{
					uint32_t result = (uint32_t)HAL_UART_Receive_IT(&huart6, (uint8_t *)&VCP_UART.rxCharBuffer, 1);
					if(result != HAL_OK)
					{
						uartNeedsRestarted[i] = true;
						AnyRestartsNeeded = true;
					}
				}
				break;
				default:
				while(1);
			}
		}
	}
	if(!AnyRestartsNeeded)
	{
		stopTim4Isr();
	}
}
#include "bsp.h"
uint32_t t4ticks = 0;
uint32_t t4seconds = 0;
void timer4TickCallback(void)
{
	restartHuartIfNeeded();
	//t4ticks++;
	t4seconds++;
	//if(t4ticks > 999)
	{
		//Toggle LED
		//bspIOToggleLED();
	//	t4ticks = 0;
	}
}

void HAL_UART_AbortCpltCallback (UART_HandleTypeDef *huart){while(1);};
void HAL_UART_AbortTransmitCpltCallback (UART_HandleTypeDef *huart){while(1);};
void HAL_UART_AbortReceiveCpltCallback (UART_HandleTypeDef *huart){while(1);};
/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
