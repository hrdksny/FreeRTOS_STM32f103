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

void ledInit(void);
// RTOS task
void vTaskLedRed(void *p);
void vTaskLedYellow(void *p);
void vTaskLedGreen(void *p);
void vTaskLedBlue(void *p);


int main(void)
{
	initialise_monitor_handles();
	printf("Starnig ..\n\r");
	long temp = 0;
	// Configure GPIO for LED
	ledInit();

	// Create LED blink task
	temp = xTaskCreate(vTaskLedRed, (const char*) "Red LED Blink", MY_TASK_STACK_SIZE, NULL, MY_TASK_PRIORITY, NULL);
	//printf("%ld\n\r",temp);
	xTaskCreate(vTaskLedYellow, (const char*) "Yellow LED Blink", 128, NULL, 1, NULL);
	xTaskCreate(vTaskLedGreen, (const char*) "Green LED Blink", 128, NULL, 1, NULL);
	//xTaskCreate(vTaskLedBlue, (const char*) "Blue LED Blink", 128, NULL, 1, NULL);
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

	// Configure PC13, PC14, PC15 as push-pull output
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void vTaskLedRed(void *p)
{
	for (;;)
	{
		GPIOC->ODR ^= GPIO_Pin_13/*14*/;
		vTaskDelay(100/portTICK_RATE_MS);
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
/*int main(void)
{

	for(;;);
}*/
