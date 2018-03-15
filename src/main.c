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
#include "MPU9250.h"
#include "Timer.h"
#include "SpiInit.h"
#include "SSD1306.h"
#include "Oleddisplay.h"

extern char c;

void ledInit(void);
// RTOS task
void vTaskLedRed(void *p);
void vTaskLedYellow(void *p);
void vTaskLedGreen(void *p);
void vTaskLedBlue(void *p);
void vTaskUart(void *p);

void ledInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
//	SPI_InitTypeDef SPI_InitStruct;

	// Configure PC13 as push-pull output
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);

//	GPIO_PinRemapConfig(GPIO_Remap_SPI1,DISABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
//	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7/*MOSI1*/ | GPIO_Pin_5 /*SCLK1*/;// | GPIO_Pin_6 /*MISO1*/;
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;/*MISO*/
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3/*4*/;/*CS*/
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
//	//GPIO_WriteBit(GPIOA, GPIO_Pin_4, 0);//(GPIOA, GPIO_Pin_6);
//	GPIO_WriteBit(GPIOA, GPIO_Pin_3/*4*/, 1);//(GPIOA, GPIO_Pin_4);
//
//	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	//SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
//	SPI_InitStruct.SPI_DataSize 		 = SPI_DataSize_8b;
//	SPI_InitStruct.SPI_CPOL 			 = SPI_CPOL_High;
//	SPI_InitStruct.SPI_CPHA 			 = SPI_CPHA_2Edge;
//	SPI_InitStruct.SPI_NSS 				 = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
//	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; /*(Fpclk/8 = 8Mhz/8 = 1Mhz)*/
//	SPI_InitStruct.SPI_FirstBit 		 = SPI_FirstBit_MSB;
//	SPI_InitStruct.SPI_Mode 			 = SPI_Mode_Master;
//	SPI_Init(SPI1, &SPI_InitStruct);
//	SPI_Cmd(SPI1, ENABLE);
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
//		vTaskDelay(1/portTICK_RATE_MS);
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
	const portTickType xDelay = 1000 / portTICK_RATE_MS;

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

void _sendData(uint8_t c)
{
	//GPIOC->ODR &= ~GPIO_Pin_4; // ReSet Pin CS//GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
	}
	SPI_I2S_SendData(SPI1, c);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
	{
	}
	//GPIOC->ODR |= GPIO_Pin_4; // Set Pin CS//GPIO_SetBits(GPIOA, GPIO_Pin_4);
}
uint8_t _readData(uint8_t* Data)
{
	uint8_t Data1 = 0;
	//GPIOC->ODR &= ~GPIO_Pin_4; // ReSet Pin CS//GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	//SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Reset);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
	{
	}
	Data1 = SPI_I2S_ReceiveData(SPI1);
	//GPIOC->ODR |= GPIO_Pin_4; // Set Pin CS//GPIO_SetBits(GPIOA, GPIO_Pin_4);
	//SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
	return Data1;
}

void WriteReg(uint8_t Add , uint8_t value)
{
	//GPIOB/*A*/->ODR &= ~GPIO_Pin_0/*4*/; // ReSet Pin CS//GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_WriteBit(GPIOA, GPIO_Pin_3/*4*/, 0);//(GPIOA, GPIO_Pin_4);
	_sendData(Add);
	_sendData(value);
	GPIO_WriteBit(GPIOA, GPIO_Pin_3/*4*/, 1);//(GPIOA, GPIO_Pin_4);
	//GPIOB/*A*/->ODR |= GPIO_Pin_0/*4*/; // Set Pin CS//GPIO_SetBits(GPIOA, GPIO_Pin_4);
	//delay_ms(10);
}

uint8_t ReadReg(uint8_t Add , uint8_t* value)
{
	uint8_t Data1 = 0;
	//GPIOB/*A*/->ODR &= ~GPIO_Pin_0/*4*/; // ReSet Pin CS//GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_WriteBit(GPIOA, GPIO_Pin_3/*4*/, 0);//(GPIOA, GPIO_Pin_4);
	_sendData(Add | READ_FLAG);
	//_sendData(0x00);
	Data1 = _readData(value);
	GPIO_WriteBit(GPIOA, GPIO_Pin_3/*4*/, 1);//(GPIOA, GPIO_Pin_4);
	//GPIOB/*A*/->ODR |= GPIO_Pin_0/*4*/; // Set Pin CS//GPIO_SetBits(GPIOA, GPIO_Pin_4);
	//delay_ms(10);
	return Data1;
}
#define MPU_InitRegNum 		21

uint8_t MPU_Init(uint8_t LP_Filter)
{
    uint8_t i = 0;
    uint8_t MPU_Init_Data[MPU_InitRegNum][2] =
    {
        {0x80, MPUREG_PWR_MGMT_1},     // Reset Device
        {0x01, MPUREG_PWR_MGMT_1},     // Clock Source()
        {0x00, MPUREG_PWR_MGMT_2},     // Enable Acc & Gyro

        {LP_Filter, MPUREG_CONFIG},    // Use DLPF set Gyroscope bandwidth 184Hz, temperature bandwidth 188Hz
        {0x18, MPUREG_GYRO_CONFIG},    // +-2000dps
        {0x08, MPUREG_ACCEL_CONFIG},   // +-4G
        {0x02, MPUREG_ACCEL_CONFIG_2}, // Set Acc Data Rates, Enable Acc LPF , Bandwidth 184Hz//changed,0-460,1-184,2-92,3-41,4-20,5-10,6-5,7-460
//		{0x02, MPUREG_SMPLRT_DIV},

        {0x30, MPUREG_INT_PIN_CFG},    //
        //{0x40, MPUREG_I2C_MST_CTRL},   // I2C Speed 348 kHz
        //{0x20, MPUREG_USER_CTRL},      // Enable AUX
        {0x20, MPUREG_USER_CTRL},       // I2C Master mode
        {0x0D, MPUREG_I2C_MST_CTRL}, //  I2C configuration multi-master  IIC 400KHz

        {AK8963_I2C_ADDR, MPUREG_I2C_SLV0_ADDR},  //Set the I2C slave addres of AK8963 and set for write.
        //{0x09, MPUREG_I2C_SLV4_CTRL},
        //{0x81, MPUREG_I2C_MST_DELAY_CTRL}, //Enable I2C delay

        {AK8963_CNTL2, MPUREG_I2C_SLV0_REG}, //I2C slave 0 register address from where to begin data transfer
        {0x01, MPUREG_I2C_SLV0_DO}, // Reset AK8963
        {0x81, MPUREG_I2C_SLV0_CTRL},  //Enable I2C and set 1 byte

        {AK8963_CNTL1, MPUREG_I2C_SLV0_REG}, //I2C slave 0 register address from where to begin data transfer
        {0x12, MPUREG_I2C_SLV0_DO}, // Register value to continuous measurement in 16bit
        {0x81, MPUREG_I2C_SLV0_CTRL},  //Enable I2C and set 1 byte

		{0x40, MPUREG_INT_ENABLE},//enable wake on interrupt
        {0xC0,MPUREG_MOT_DETECT_CTRL},
		{0x7F, MPUREG_WOM_THR},//Added to anable wake on thresold {0xff, MPUREG_WOM_THR},//Added to anable wake on thresold
        {0x30, MPUREG_INT_PIN_CFG},//interrupt pin config pin high until interrupt clear
    };

    //spi.format(8,0); //consider
    //spi.frequency(1000000); //consider

    for(i=0; i<MPU_InitRegNum; i++)
    {
    	WriteReg(MPU_Init_Data[i][1], MPU_Init_Data[i][0]);
    	//MPU_WriteReg(MPU_Init_Data[i][1], MPU_Init_Data[i][0]);
    	//Spi_WriteReg(MPU_Init_Data[i][1], MPU_Init_Data[i][0]);
        //Delay_ms(1);
    	//delay_ms(1);
    	//wait(0.001);  //I2C must slow down the write speed, otherwise it won't work
    }

    //Set_Acc_Scale(2);
    //Set_Gyro_Scale(250);

    //AK8963_calib_Magnetometer();  //Can't load this function here , strange problem?
    return 0;
}

int main(void)
{
	int temp = 0;
	initialise_monitor_handles();
	printf("Startnig %ld ..\n\r",SystemCoreClock);

	//UART Port Init
	usart1_init();
	// Configure GPIO for LED
	ledInit();
	printf("1\n\r");
	//timer_start();
	printf("2\n\r");
	spiSetup(OLED_SPI);
	printf("3\n\r");
	ssd1306Init();
	printf("4\n\r");
	OLED_PrintfSetColumnRow(0, 4, LARGE_FONT);
	printf("5\n\r");
	OLED_Printf("Hi HARDIK");
	printf("6\n\r");
//	MPU_Init(BITS_DLPF_CFG_20HZ);
//	temp = ReadReg(MPUREG_WHOAMI, 0);
//	printf("I_AM %d ..\n\r",temp);
//	temp = ReadReg(0x6B, 0);
//	printf("I_AM %d ..\n\r",temp);
	// Create LED blink task
	xTaskCreate(vTaskLedRed, (const char*) "Red LED Blink", MY_TASK_STACK_SIZE, NULL, MY_TASK_PRIORITY, NULL);
	//printf("%ld\n\r",temp);
	//xTaskCreate(vTaskLedYellow, (const char*) "Yellow LED Blink", 128, NULL, 1, NULL);
	//xTaskCreate(vTaskLedGreen, (const char*) "Green LED Blink", 128, NULL, 1, NULL);
	//xTaskCreate(vTaskLedBlue, (const char*) "Blue LED Blink", 128, NULL, 1, NULL);
	xTaskCreate(vTaskUart, (const char*) "UART Task", MY_TASK_STACK_SIZE, NULL, (MY_TASK_PRIORITY + 2), NULL);
	// Start RTOS scheduler
	vTaskStartScheduler();

	while(1)
	{
	}

	return 0;
}
/*int main(void)
{

	for(;;);
}*/
