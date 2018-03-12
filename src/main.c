/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


//#include "stm32f10x.h"

#define MY_TASK_STACK_SIZE 		configMINIMAL_STACK_SIZE
#define MY_TASK_PRIORITY 		tskIDLE_PRIORITY + 2
void __attribute__((weak)) initialise_monitor_handles(void);

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include <stdlib.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"

extern char c;

void ledInit(void);
// RTOS task
void vTaskLedRed(void *p);
void vTaskLedYellow(void *p);
void vTaskLedGreen(void *p);
void vTaskLedBlue(void *p);
void vTaskUart(void *p);


int main(void)
{
	//initialise_monitor_handles();
	printf("Starnig %d ..\n\r",SystemCoreClock);

	//UART Port Init
	usart1_init();
	// Configure GPIO for LED
	ledInit();

	// Create LED blink task
	xTaskCreate(vTaskLedRed, (const char*) "Red LED Blink", MY_TASK_STACK_SIZE, NULL, MY_TASK_PRIORITY, NULL);
	//printf("%ld\n\r",temp);
	//xTaskCreate(vTaskLedYellow, (const char*) "Yellow LED Blink", 128, NULL, 1, NULL);
	//xTaskCreate(vTaskLedGreen, (const char*) "Green LED Blink", 128, NULL, 1, NULL);
	//xTaskCreate(vTaskLedBlue, (const char*) "Blue LED Blink", 128, NULL, 1, NULL);
	xTaskCreate(vTaskUart, (const char*) "UART Task", 128, NULL, 1, NULL);
	// Start RTOS scheduler
	vTaskStartScheduler();

	while(1)
	{
	}

	return 0;
}

void ledInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// Configure PC13, PC14, PC15 as push-pull output
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);

	GPIO_PinRemapConfig(GPIO_Remap_SPI1,DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7/*MOSI1*/ | GPIO_Pin_5 /*SCLK1*/;// | GPIO_Pin_6 /*MISO1*/;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;/*MISO*/
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;/*CS*/
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, 0);//(GPIOA, GPIO_Pin_6);
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, 1);//(GPIOA, GPIO_Pin_4);

	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
	SPI_InitStruct.SPI_DataSize 		 = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL 			 = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA 			 = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS 				 = SPI_NSS_Soft;// | SPI_NSSInternalSoft_Set;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; /*(Fpclk/8 = 8Mhz/8 = 1Mhz)*/
	SPI_InitStruct.SPI_FirstBit 		 = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode 			 = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

void vTaskLedRed(void *p)
{
	for (;;)
	{
		//c = usart1_getc();
		if(c == '1')
			GPIOC->ODR &= ~GPIO_Pin_13/*14*/;
		else
			GPIOC->ODR |= GPIO_Pin_13/*14*/;
//		GPIOC->ODR ^= GPIO_Pin_13/*14*/;
//		vTaskDelay(100/portTICK_RATE_MS);
	}
}

void vTaskLedYellow(void *p)
{
	for (;;)
	{
		GPIOC->ODR ^= GPIO_Pin_13/*15*/;
		vTaskDelay(500/portTICK_RATE_MS);
	}
}

void vTaskLedGreen(void *p)
{
	for (;;)
	{
		GPIOC->ODR ^= GPIO_Pin_13;
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}

void vTaskLedBlue(void *p)
{
	for (;;)
	{
		GPIOC->ODR ^= GPIO_Pin_13;
		vTaskDelay(1500/portTICK_RATE_MS);
	}
}
void vTaskUart(void *p)
{
	// Block for 500ms.
	const portTickType xDelay = 500 / portTICK_RATE_MS;

	for(;;) {
//		usart1_puts("FreeRTOS V9.0.0 demo on STM32F103VCT6 STM32F103VC\r\n");
//		usart1_puts(" 512k flash, 64k ram ..........\r\n");
		usart1_puts("Print ..........\r\n");
		vTaskDelay(xDelay);
/*		r = usart1_getc();
		if(r == '1')
			GPIOC->ODR ^= GPIO_Pin_1314;*/
	}
}
/*int main(void)
{

	for(;;);
}*/
