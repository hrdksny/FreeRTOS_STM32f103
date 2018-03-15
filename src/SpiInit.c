/*
 * SpiInit.c

 *
 *  Created on: 02-May-2017
 *      Author: hardik
 */
#include "SpiInit.h"
//#include "diag/Trace.h"

void csInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_WriteBit(GPIOC, GPIO_Pin_3, 1);
}

void spiSetup(SPI_TypeDef *SPIx)
{
	SPI_InitTypeDef	SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;


	SPI_StructInit(&SPI_InitStructure);
	GPIO_StructInit(&GPIO_InitStructure);

	//RCC_PCLK1Config(RCC_HCLK_Div8);
	//RCC_PCLK2Config(RCC_HCLK_Div8);

	if(SPIx == SPI1)
	{

		/* Enable GPIO clock for SPI2 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2ENR_SPI1EN, ENABLE);

		/* Enable SPIz Periph clock */
		RCC_APB2PeriphClockCmd(SPI1_MODULE_CLK, ENABLE);

		//RCC_APB2PeriphClockCmd(SPI1_PORT_CLK,ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = SPI1_SCK | SPI1_MOSI;//GPIO_Pin_5;//SCK
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = SPI1_MISO;//GPIO_Pin_6;//MISO

		GPIO_Init(GPIOA,&GPIO_InitStructure);
//
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//MOSI
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//
//		GPIO_Init(GPIOA,&GPIO_InitStructure);
//		trace_printf("SPI1");
	}
	else if(SPIx == SPI2)
	{

		/* Enable GPIO clock for SPI2 */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		/* Enable SPI2 Periph clock */
		RCC_APB1PeriphClockCmd(SPI2_MODULE_CLK, ENABLE);

		//RCC_APB2PeriphClockCmd(SPI2_PORT_CLK,ENABLE);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = SPI2_SCK | SPI2_MOSI;//GPIO_Pin_13;//SCK
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(GPIOB,&GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = SPI2_MISO;//GPIO_Pin_14;//MISO

		GPIO_Init(GPIOB,&GPIO_InitStructure);

//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//MOSI
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//
//		GPIO_Init(GPIOB,&GPIO_InitStructure);
	}
	else
	{
		//trace_printf("return");
		return;
	}

	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;

	SPI_Init(SPIx,&SPI_InitStructure);

	SPI_Cmd(SPIx, ENABLE);
	//trace_printf("SPI Config Exit \n");
}


int spiReadWrite(SPI_TypeDef *SPIx, uint8_t *rxBuf, const uint8_t *txBuf, int count)
{
	int i = 0;

	SPIx->CR1 = SPIx->CR1 | SPI_BaudRatePrescaler_64;
	//SPIx->CR1 = (SPIx->CR1 & (~SPI_BaudRatePrescaler_256)) | SPI_BaudRatePrescaler_32;

	for(i=0;i<count;i++)
	{
		if(txBuf)
		{
			SPI_I2S_SendData(SPIx,*txBuf++);
		}
		else
		{
			SPI_I2S_SendData(SPIx,0xff);
		}
	}
	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);

	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == SET)
	{
		*rxBuf = (uint8_t)SPI_I2S_ReceiveData(SPIx);
		rxBuf++;
	}

	return i;
}

