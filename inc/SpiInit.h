

#ifndef SPIINIT_H_
#define SPIINIT_H_

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define  OLED_SPI			SPI1
#define  SPI1_GPIO			GPIOA
#define  SPI1_MODULE_CLK	RCC_APB2Periph_SPI1
#define  SPI1_PORT_CLK		RCC_APB2Periph_GPIOA
#define  SPI1_MOSI			GPIO_Pin_7
#define  SPI1_MISO			GPIO_Pin_6
#define	 SPI1_SCK			GPIO_Pin_5

#define  SPI2_GPIO			GPIOB
#define  SPI2_MODULE_CLK	RCC_APB1Periph_SPI2
#define  SPI2_PORT_CLK		RCC_APB2Periph_GPIOB
#define  SPI2_MOSI			GPIO_Pin_15
#define  SPI2_MISO			GPIO_Pin_14
#define	 SPI2_SCK			GPIO_Pin_13


extern void csInit(void);
extern void spiSetup(SPI_TypeDef *SPIx);
extern int spiReadWrite(SPI_TypeDef *SPIx, uint8_t *rxBuf, const uint8_t *txBuf, int count);

#endif
