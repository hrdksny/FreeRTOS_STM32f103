/*
 * SSD1306.c
 *
 *  Created on: 04-May-2017
 *      Author: hardik
 */
/*
 * SSD1306.c
 *
 *  Created on: 06-Oct-2016
 *      Author: hardik.soni
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Font_14x15.h"
#include "Font_5x7.h"
#include "Oleddisplay.h"
#include "SSD1306.h"
#include "SpiInit.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"
#include "Timer.h"
#include "stm32f10x.h"
#include "usart.h"
//#include "diag/Trace.h"

#define  SSD1306_SELECT			(GPIO_WriteBit(GPIOA, SSD1306_CS, 0))
#define  SSD1306_DESELECT		(GPIO_WriteBit(GPIOA, SSD1306_CS, 1))
#define  SSD1306_MODE_CMD		(GPIO_WriteBit(GPIOA, SSD1306_DATACMD, 0))
#define  SSD1306_MODE_DATA		(GPIO_WriteBit(GPIOA, SSD1306_DATACMD, 1))
#define  SSD1306_RESET_HIGH		(GPIO_WriteBit(GPIOA, SSD1306_RESET, 1))
#define  SSD1306_RESET_LOW		(GPIO_WriteBit(GPIOA, SSD1306_RESET, 0))

const uint8_t init_cmd_array[]={
		0xAE,			// DISPLAY OFF

		0xD5,			// SET OSC FREQUENY
		0xF0,			// divide ratio = 1 (bit 3-0), OSC (bit 7-4) //Max Osc Frequency

		0xA8,			// SET MUX RATIO
		0x3F,			// 64MUX

		0xD3,			// SET DISPLAY OFFSET
		0x00,			// offset = 0

		0x40,			// set display start line, start line = 0

		0x8D,			// ENABLE CHARGE PUMP REGULATOR
		0x14,			//

		0x20,			// SET MEMORY ADDRESSING MODE
		0x02,			// Page Addressing mode 0x00,//

		0xA1,			// set segment re-map, column address 127 is mapped to SEG0 //(A1- remap/A0-normal)//Horizontal flip
		0xC0,			// set COM/Output scan direction, remapped mode (COM[N-1] to COM0)(C0-remap,C8 normal)//vertical flip

		0xDA,			// SET COM PINS HARDWARE CONFIGURATION
		0x12,			// alternative COM pin configuration

		0x81,			// SET CONTRAST CONTROL
		0xFF,			//

		0xD9,			// SET PRE CHARGE PERIOD
		0xF1,			//

		0xDB,			// SET V_COMH DESELECT LEVEL
		0x40,			//

		0xA4,			// DISABLE ENTIRE DISPLAY ON
		0xA6,			// NORMAL MODE (A7 for inverse display)
		0xAF			// DISPLAY ON
};

uint8_t Buffer[SSD1306_LCDHEIGHT*SSD1306_LCDWIDTH] = {'\0'};

/* Function    : SSD1306PinSetup()
 * Description : Sets up the pins to the OLED display.
 * Input       : None
 * Output      : None
 *
 */
void ssd1306PinSetup(void)
{
	//Setup GPIO Pins for CS, D/C and RES pins of Oled Display Controller
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = SSD1306_CS | SSD1306_DATACMD | SSD1306_RESET;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOA,&GPIO_InitStructure);

//   OLED_CS_PORT_DIR |= OLED_CS;
//   OLED_DC_DIR |= OLED_DC;
//   OLED_RES_DIR |= OLED_RES;
	SSD1306_DESELECT;
	SSD1306_MODE_CMD;
	SSD1306_RESET_HIGH;
	//timer_sleep(10);
}

/* Function    : SSD1306Init()
 * Description : Initializes the OLED display
 * Input       : None
 * Output      : None
 */
void ssd1306Init(void)
{
	uint8_t i = 0;
	// Pull all pins low except CS
	//OLED_SPI->CR1 = OLED_SPI->CR1 | SPI_BaudRatePrescaler_64;

	ssd1306PinSetup();
	usart1_puts("A\r\n");

	SSD1306_DESELECT;//OLED_DESELECT;
	usart1_puts("B\r\n");
#if SPI_9BIT
#else
//	SSD1306_MODE_CMD;//   OLED_COMMAND_MODE;	//OLED_DC_OUT &= ~OLED_DC;
#endif
//SSD1306_RESET_HIGH;
SSD1306_RESET_LOW;//   OLED_RES_OUT &= ~OLED_RES;
usart1_puts("C\r\n");
//
//   // Delay for 200ms at 16Mhz
// timer_sleep(2);

 SSD1306_RESET_HIGH;//   OLED_RES_OUT |= OLED_RES;
 usart1_puts("D\r\n");

 // Delay for 200ms at 16Mhz
// timer_sleep(200);
//
//SSD1306_RESET_HIGH;//   OLED_RES_OUT &= ~OLED_RES;


//   SSD1306_DESELECT;//OLED_DESELECT;
#if SPI_9BIT
    OLED_Spi_Mode = CMD;
#else
 //   SSD1306_MODE_CMD;//OLED_COMMAND_MODE;
#endif
    SSD1306_SELECT;//OLED_SELECT;
    //usart1_puts("E\r\n");
    for(i=0;i<25;i++)
    {
    	while (!(OLED_SPI->SR & (SPI_I2S_FLAG_TXE)));
    	OLED_SPI->DR = init_cmd_array[i];

    	while (OLED_SPI->SR & (SPI_I2S_FLAG_BSY));
//    	SPI_I2S_SendData(OLED_SPI,init_cmd_array[i]);
//        while(SPI_I2S_GetFlagStatus(OLED_SPI,SPI_SR_BSY) == SET);
    }
    usart1_puts("F\r\n");
	//SPI.Write((uint8_t*)init_cmd_array,sizeof(init_cmd_array));

    SSD1306_DESELECT;//OLED_DESELECT;
    usart1_puts("G\r\n");

   Fill_RAM(0x00);				// Clear Screen
   usart1_puts("H\r\n");
//   memset(OledDisplayBuffer,'\0',OLED_BUFFER_SIZE);
}

/* Function    : ssd1306SendCommand( char c )
 * Description : Sends a command to the OLED display
 * Input       : Command
 * Output      : None
 */
void ssd1306SendCommand(uint8_t c)
{

	//OLED_SPI->CR1 = OLED_SPI->CR1 | SPI_BaudRatePrescaler_64;
	//SSD1306_DESELECT;//OLED_DESELECT;
#if SPI_9BIT
	OLED_Spi_Mode = CMD;
#else
	SSD1306_MODE_CMD;//OLED_COMMAND_MODE;
#endif
	SSD1306_SELECT;//OLED_SELECT;
	OLED_SPI->DR = c;
	while (!(OLED_SPI->SR & (SPI_I2S_FLAG_TXE)));
	while (OLED_SPI->SR & (SPI_I2S_FLAG_BSY));
//	SPI_I2S_SendData(OLED_SPI,c);//SPI.Write((uint8_t*)&c,1);
//	while(SPI_I2S_GetFlagStatus(OLED_SPI,SPI_SR_TXE) == RESET);
	SSD1306_DESELECT;//OLED_DESELECT;
}

/* Function    : ssd1306SendData( char c )
 * Description : Sends data to the OLED display
 * Input       : Data
 * Output      : None
 */
void ssd1306SendData(uint8_t c )
{
	//OLED_SPI->CR1 = OLED_SPI->CR1 | SPI_BaudRatePrescaler_64;
	//SSD1306_DESELECT;//OLED_DESELECT;
#if SPI_9BIT
	OLED_Spi_Mode = DATA;
#else
	SSD1306_MODE_DATA;//OLED_DATA_MODE;
#endif
	SSD1306_SELECT;//OLED_SELECT;
	OLED_SPI->DR = c;
	while (!(OLED_SPI->SR & (SPI_I2S_FLAG_TXE)));
	while (OLED_SPI->SR & (SPI_I2S_FLAG_BSY));
//	SPI_I2S_SendData(OLED_SPI,c);//SPI.Write((uint8_t*)&c,1);
//	while(SPI_I2S_GetFlagStatus(OLED_SPI,SPI_SR_TXE) == RESET);
	SSD1306_DESELECT;//OLED_DESELECT;
}

/* Function    : Fill_RAM( unsigned char c )
 * Description : Fills the OLED with one particular data
 * Input       : Data
 * Output      : None
 */
void Fill_RAM(unsigned char Data)
{
   unsigned char i,j;

   for(i=0;i<8;i++)
   {
      Set_Start_Page(i);
      Set_Start_Column(0x00);

      for(j=0;j<128;j++)
      {
         ssd1306SendData((uint16_t)Data);
      }
   }
}
//
void Fill_RAM_Whole(const unsigned char *Data, uint8_t page, uint8_t colon, uint8_t row)
{
   unsigned char i,j;

   for(i=0;i<page;i++)
   {
      Set_Start_Page(i);
      Set_Start_Column(colon);

      for(j=0;j<row;j++)
      {
         ssd1306SendData((uint16_t)Data[j]);
         //Data++;
      }
   }
}

void Fill_Pixel(const unsigned char *Data, uint8_t page_start,uint8_t page_end, uint8_t colon, uint8_t row)

{
   unsigned char i,j;

   for(i=page_start;i<page_end;i++)
   {
      Set_Start_Page(i);
      Set_Start_Column(colon);

      for(j=0;j<row;j++)
      {
         ssd1306SendData((uint16_t)Data[j]);
         //Data++;
      }
   }
}

void Fill_DataPixel(const unsigned char Data, uint8_t page_start,uint8_t page_end, uint8_t colon, uint8_t row)
{
   unsigned char i,j;

   for(i=page_start;i<page_end;i++)
   {
      Set_Start_Page(i);
      Set_Start_Column(colon);
      for(j=0;j<row;j++)
      {
         ssd1306SendData((uint16_t)Data);
      }
   }
}

/* Function    : Fill_RAM_PAGE
 * Description : Fills an OLED page with a particular data
 * Input       : page no.[0-7], Data
 * Output      : None
 */
void Fill_RAM_PAGE(unsigned char page, unsigned char Data)
{
   unsigned char j;

   Set_Start_Page(page);
   Set_Start_Column(0x00);

   for(j=0;j<128;j++)
   {
      ssd1306SendData((uint16_t)Data);
   }
}

/* Function    : Fill_RAM_FONT_SMALL
 * Description : Writes a charcter on the OLED display at a column, row.
 *               with a small font.
 * Input       : character, column, row
 * Output      : None
 */
void Fill_RAM_FONT_SMALL(  unsigned char number,
                           unsigned char column,
                           unsigned char row )
{
	int j=0;
	Set_Start_Page(row);
	Set_Start_Column(column);

	for(j=0;j<SMALL_FONT_WIDTH;j++)
	{
		ssd1306SendData((uint16_t)data_table_SMALL[j+(number*SMALL_FONT_WIDTH)]);
	}
}

/* Function    : Fill_RAM_FONT
 * Description : Writes a charcter on the OLED display at a column, row.
 *               with a large font.
 * Input       : character, column, row, span and width
 * Output      : None
 */
void Fill_RAM_FONT(  unsigned char number,
                     unsigned char column,
                     unsigned char row,
                     unsigned char span,
                     unsigned char width )
{
	int i,j,test;
	int adder = 0;
	int jump = 0;

   jump = (number - 32);
	for(i=row;i>=row-span+1;i--)
	{
		Set_Start_Page((unsigned char)i);
		Set_Start_Column(column);

		for(j=0;j<width;j++)
		{
			test = 	j+(adder*width)+(jump*28);
			ssd1306SendData((uint16_t)data_table_LARGE[test]);
		}
		adder++;
	}
}

/* Function    : Set_Start_Column
 * Description : Sets a start column to start writing at.
 * Input       : column[0-127]
 * Output      : None
 */
void Set_Start_Column(unsigned char d)
{
	// Set Lower Column Start Address for Page Addressing Mode. Default => 0x00
   ssd1306SendCommand(0x00+d%16);

	// Set Higher Column Start Address for Page Addressing Mode. Default => 0x10
   ssd1306SendCommand((uint8_t)(0x10+d/16));
}

/* Function    : Set_Start_Page
 * Description : Sets a start page to start writing at.
 * Input       : column[0-7]
 * Output      : None
 */
void Set_Start_Page(unsigned char d)
{
	// Set Page Start Address for Page Addressing Mode. Default => 0xB0 (0x00)
   ssd1306SendCommand(0xB0|d);

}

void Set_Addressing_Mode(unsigned char d)
{
	// Set Memory Addressing Mode
   ssd1306SendCommand(0x20);
	//   Default => 0x02
   ssd1306SendCommand(d);
   //     0x00 => Horizontal Addressing Mode
   //     0x01 => Vertical Addressing Mode
   //     0x02 => Page Addressing Mode
}


void Set_Column_Address(unsigned char a, unsigned char b)
{
	// Set Column Address
   ssd1306SendCommand(0x21);
	//   Default => 0x00 (Column Start Address)
   ssd1306SendCommand(a);
	//   Default => 0x7F (Column End Address)
   ssd1306SendCommand(b);
}


void Set_Page_Address(unsigned char a, unsigned char b)
{
	// Set Page Address
   ssd1306SendCommand(0x22);
	//   Default => 0x00 (Page Start Address)
   ssd1306SendCommand(a);
	//   Default => 0x07 (Page End Address)
   ssd1306SendCommand(b);
}


void Set_Start_Line(unsigned char d)
{
	// Set Display Start Line
   ssd1306SendCommand(0x40|d);
	//   Default => 0x40 (0x00)
}


void Set_Contrast_Control(unsigned char d)
{
	// Set Contrast Control
   ssd1306SendCommand(0x81);
	//   Default => 0x7F
   ssd1306SendCommand(d);
}


void Set_Charge_Pump(unsigned char d)
{
	// Set Charge Pump
   ssd1306SendCommand(0x8D);
	//   Default => 0x10.
   ssd1306SendCommand(0x10|d);
	//     0x10 (0x00) => Disable Charge Pump
	//     0x14 (0x04) => Enable Charge Pump
}


void Set_Segment_Remap(unsigned char d)
{
	// Set Segment Re-Map
   ssd1306SendCommand(0xA0|d);
   //   Default => 0xA0
   //     0xA0 (0x00) => Column Address 0 Mapped to SEG0
   //     0xA1 (0x01) => Column Address 0 Mapped to SEG127
}


void Set_Entire_Display(unsigned char d)
{
	// Set Entire Display On / Off
   ssd1306SendCommand(0xA4|d);
   //   Default => 0xA4
   //     0xA4 (0x00) => Normal Display
   //     0xA5 (0x01) => Entire Display On
}


void Set_Inverse_Display(unsigned char d)
{
	// Set Inverse Display On/Off
   ssd1306SendCommand(0xA6|d);
	//   Default => 0xA6
	//     0xA6 (0x00) => Normal Display
	//     0xA7 (0x01) => Inverse Display On
}


void Set_Multiplex_Ratio(unsigned char d)
{
	// Set Multiplex Ratio
   ssd1306SendCommand(0xA8);
	ssd1306SendCommand(d);
   //   Default => 0x3F (1/64 Duty)
}


void Set_Display_On_Off(unsigned char d)
{
	// Set Display On/OffSSD1306
   ssd1306SendCommand(0xAE|d);
	//   Default => 0xAE
	//     0xAE (0x00) => Display Off
	//     0xAF (0x01) => Display On
}


void Set_Common_Remap(unsigned char d)
{
	// Set COM Output Scan Direction
   ssd1306SendCommand(0xC0|d);
   //   Default => 0xC0
   //     0xC0 (0x00) => Scan from COM0 to 63
   //     0xC8 (0x08) => Scan from COM63 to 0
}


void Set_Display_Offset(unsigned char d)
{
	// Set Display Offset
   ssd1306SendCommand(0xD3);
	//   Default => 0x00
   ssd1306SendCommand(d);
}


void Set_Display_Clock(unsigned char d)
{
	// Set Display Clock Divide Ratio / Oscillator Frequency
   ssd1306SendCommand(0xD5);
	ssd1306SendCommand(d);
   //   Default => 0x80
	//     D[3:0] => Display Clock Divider
	//     D[7:4] => Oscillator Frequency
}


void Set_Precharge_Period(unsigned char d)
{
	// Set Pre-Charge Period
   ssd1306SendCommand(0xD9);
	ssd1306SendCommand(d);
   //   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
	//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
	//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
}


void Set_Common_Config(unsigned char d)
{
	// Set COM Pins Hardware Configuration
   ssd1306SendCommand(0xDA);
	ssd1306SendCommand(0x02|d);
   //   Default => 0x12 (0x10)
	//     Alternative COM Pin Configuration
	//     Disable COM Left/Right Re-Map
}


void Set_VCOMH(unsigned char d)
{
	// Set VCOMH Deselect Level
   ssd1306SendCommand(0xDB);
	ssd1306SendCommand(d);
   //   Default => 0x20 (0.77*VCC)
}

void fillPixel(uint8_t x, uint8_t y,uint8_t invert)
{
	uint8_t pixelData = 0;
	if(x>SSD1306_LCDWIDTH || y>SSD1306_LCDHEIGHT)
	{
		//trace_printf("x or y value is out of limit");
	}
	else
	{
		// Set Column Address
		Set_Start_Column(x);
		// Set Page Address
		ssd1306SendCommand(0xB0 | (uint8_t)(y>>3));

		pixelData = (uint8_t)(0x01 << (y%8));
		switch (invert) {
			case 0:
				ssd1306SendData(0x00);
				break;
			case 1:
				ssd1306SendData(pixelData);
				break;
			default:
				break;
		}
	}
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint8_t clr)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = (int16_t)abs(x1 - x0);
  dy = (int16_t)abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
    	fillPixel((uint8_t)y0, (uint8_t)x0,clr);
    } else {
    	fillPixel((uint8_t)x0, (uint8_t)y0,clr);
    }
    err = (int16_t)(err - dy);
    if (err < 0) {
      y0 = (int16_t)(y0+ystep);
      err = (int16_t)(err+dx);
    }
  }
}


void drawCircle(int16_t x0, int16_t y0, int16_t radius)
{
	int16_t x = 0, y = radius;
	int16_t dp = (int16_t)(1 - radius);
	do
	{
		if (dp < 0)
			dp = (int16_t)(dp + 2 * (++x) + 3);
		else
			dp = (int16_t)(dp + 2 * (++x) - 2 * (--y) + 5);

		fillPixel((uint8_t)(x0 + x), (uint8_t)(y0 + y),1);     //For the 8 octants
		fillPixel((uint8_t)(x0 - x), (uint8_t)(y0 + y),1);
		fillPixel((uint8_t)(x0 + x), (uint8_t)(y0 - y),1);
		fillPixel((uint8_t)(x0 - x), (uint8_t)(y0 - y),1);
		fillPixel((uint8_t)(x0 + y), (uint8_t)(y0 + x),1);
		fillPixel((uint8_t)(x0 - y), (uint8_t)(y0 + x),1);
		fillPixel((uint8_t)(x0 + y), (uint8_t)(y0 - x),1);
		fillPixel((uint8_t)(x0 - y), (uint8_t)(y0 - x),1);

	} while (x < y);

	fillPixel((uint8_t)(x0 + radius), (uint8_t)y0,1);
	fillPixel((uint8_t)x0, (uint8_t)(y0 + radius),1);
	fillPixel((uint8_t)(x0 - radius), (uint8_t)y0,1);
	fillPixel((uint8_t)x0, (uint8_t)(y0 - radius),1);
}

void drawCircleQuads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads)
{
  int16_t x = 0, y = radius;
  int16_t dp = (int16_t)(1 - radius);
  while (x < y)
  {
    if (dp < 0)
      dp = (int16_t)(dp + 2 * (++x) + 3);
    else
      dp = (int16_t)(dp + 2 * (++x) - 2 * (--y) + 5);
    if (quads & 0x1)
    {
      fillPixel((uint8_t)(x0 + x),(uint8_t)(y0 - y),1);
      fillPixel((uint8_t)(x0 + y),(uint8_t)(y0 - x),1);
    }
    if (quads & 0x2)
    {
    	fillPixel((uint8_t)(x0 - y),(uint8_t)(y0 - x),1);
    	fillPixel((uint8_t)(x0 - x), (uint8_t)(y0 - y),1);
    }
    if (quads & 0x4)
    {
    	fillPixel((uint8_t)(x0 - y), (uint8_t)(y0 + x),1);
    	fillPixel((uint8_t)(x0 - x), (uint8_t)(y0 + y),1);
    }
    if (quads & 0x8)
    {
    	fillPixel((uint8_t)(x0 + x), (uint8_t)(y0 + y),1);
    	fillPixel((uint8_t)(x0 + y), (uint8_t)(y0 + x),1);
    }
  }
  if ((quads & 0x1) && (quads & 0x8))
  {
	  fillPixel((uint8_t)(x0 + radius), (uint8_t)y0,1);
  }
  if ((quads & 0x4) && (quads & 0x8))
  {
	  fillPixel((uint8_t)x0, (uint8_t)(y0 + radius),1);
  }
  if ((quads & 0x2) && (quads & 0x4))
  {
	  fillPixel((uint8_t)(x0 - radius), (uint8_t)y0,1);
  }
  if ((quads & 0x1) && (quads & 0x2))
  {
	  fillPixel((uint8_t)x0, (uint8_t)(y0 - radius),1);
  }
}

void drawRect(int16_t x, int16_t y, int16_t width, int16_t height)
{
	horizontalLine((uint8_t)x,(uint8_t)y,(uint8_t)width);
	horizontalLine((uint8_t)x,(uint8_t)(y+height),(uint8_t)width);
	verticalLine((uint8_t)x,(uint8_t)y,(uint8_t)height);
	verticalLine((uint8_t)(x + width),(uint8_t)y,(uint8_t)height);
}

void fillRect(int16_t xMove, int16_t yMove, int16_t width, int16_t height)
{
  for (int16_t x = xMove; x < xMove + width; x++)
  {
    drawVerticalLine(x, yMove, height);
  }
  Fill_Pixel(Buffer,0,8,0,127);
}

void fillCircle(int16_t x0, int16_t y0, int16_t radius)
{
	int16_t x = 0, y = radius;
	int16_t dp = (int16_t)(1 - radius);
	do {
		if (dp < 0)
			dp = (int16_t)(dp + 2 * (++x) + 3);
		else
			dp = (int16_t)(dp + 2 * (++x) - 2 * (--y) + 5);

    drawHorizontalLine((int16_t)(x0 - x), (int16_t)(y0 - y), (int16_t)(2*x));
    drawHorizontalLine((int16_t)(x0 - x), (int16_t)(y0 + y), (int16_t)(2*x));
    drawHorizontalLine((int16_t)(x0 - y), (int16_t)(y0 - x), (int16_t)(2*y));
    drawHorizontalLine((int16_t)(x0 - y), (int16_t)(y0 + x), (int16_t)(2*y));

	} while (x < y);
  drawHorizontalLine((int16_t)(x0 - radius), (int16_t)y0, (int16_t)(2 * radius));

  Fill_Pixel(Buffer,0,8,0,127);
}

void drawHorizontalLine(int16_t x, int16_t y, int16_t length)
{
  if (y < 0 || y >= SSD1306_LCDHEIGHT) { return; }

  if (x < 0) {
    length = (int16_t)(length + x);
    x = 0;
  }

  if ( (x + length) > SSD1306_LCDWIDTH) {
    length = (int16_t)(SSD1306_LCDWIDTH - x);
  }

  if (length <= 0) { return; }

  uint8_t * bufferPtr = Buffer;
  bufferPtr += (y >> 3) * SSD1306_LCDWIDTH;
  bufferPtr += x;

  uint8_t drawBit = (uint8_t)(1 << (y & 7));

  while (length--)
  {
	  *bufferPtr++ |= drawBit;
  }
  Fill_Pixel(Buffer,0,8,0,127);
}

void drawVerticalLine(int16_t x, int16_t y, int16_t length)
{
  if (x < 0 || x >= SSD1306_LCDWIDTH) return;

  if (y < 0)
  {
    length = (int16_t)(length+y);
    y = 0;
  }

  if ( (y + length) > SSD1306_LCDHEIGHT)
  {
    length = (int16_t)(SSD1306_LCDHEIGHT - y);
  }

  if (length <= 0) return;


  uint8_t yOffset = y & 7;
  uint8_t drawBit;
  uint8_t *bufferPtr = Buffer;

  bufferPtr += (y >> 3) * SSD1306_LCDHEIGHT;
  bufferPtr += x;

  if (yOffset)
  {
    yOffset = (uint8_t)(8 - yOffset);
    drawBit = (uint8_t)(~(0xFF >> (yOffset)));

    if (length < yOffset)
    {
      drawBit = (uint8_t)(drawBit & (0xFF >> (yOffset - length)));
    }

  *bufferPtr |=  drawBit;

  if (length < yOffset) return;

  length = (int16_t)(length -yOffset);
  bufferPtr += SSD1306_LCDWIDTH;
  }

  if(length >= 8)
  {
        drawBit = 0xFF;
        do
        {
          *bufferPtr = drawBit;
          bufferPtr += SSD1306_LCDWIDTH;
          length = (int16_t)(length-8);
        } while (length >= 8);
  }

  if (length > 0)
  {
    drawBit = (uint8_t)((1 << (length & 7)) - 1);
    *bufferPtr |=  drawBit;
  }
}

void verticalLine(uint8_t x0,uint8_t y0, uint8_t length)
{
	uint8_t remainder = 0, pages = 0, i=0;
	Set_Start_Column(x0);
	Set_Start_Page(y0>>3);
	uint8_t pixelByte = (uint8_t)(0xFF >> (y0%8));
	ssd1306SendData(pixelByte);
	length = (uint8_t)(length - (y0%8));
	remainder = length % 8;
	pages = length >> 3;
	for(i=1;i<=pages;i++)
	{
		Set_Start_Column(x0);
		Set_Start_Page((uint8_t)((y0>>3) + i));
		ssd1306SendData(0xff);
	}
	Set_Start_Column(x0);
	Set_Start_Page((uint8_t)((y0>>3) + i));
	pixelByte = (uint8_t)(~(0xFF >> remainder));
	ssd1306SendData(pixelByte);
}

void horizontalLine(uint8_t x0,uint8_t y0, uint8_t length)
{
	uint8_t i=0;
	Set_Start_Column(x0);
	Set_Start_Page(y0>>3);
	for(i=0;i<=length;i++)
	{
		fillPixel((uint8_t)(x0+i),y0,1);
	}
}

