

#ifndef SSD1306_H_
#define SSD1306_H_

#include "stm32f10x.h"

#define _swap_int16_t(a, b) 		{ int16_t t = a; a = b; b = t; }
#define SSD1306_PORT				GPIOA
#define SSD1306_CS					GPIO_Pin_2//(PA2)
#define SSD1306_RESET				GPIO_Pin_3//(PA3)
#define SSD1306_DATACMD				GPIO_Pin_4//(PA4)
#define SSD1306_LCDWIDTH 			128
#define SSD1306_LCDHEIGHT 			64
#define OLED_BUFFER_SIZE			((SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT)/8)


#define SSD1306_SETCONTRAST 		0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 		0xA5
#define SSD1306_NORMALDISPLAY 		0xA6
#define SSD1306_INVERTDISPLAY 		0xA7
#define SSD1306_DISPLAYOFF 			0xAE
#define SSD1306_DISPLAYON 			0xAF

#define SSD1306_SETDISPLAYOFFSET 	0xD3
#define SSD1306_SETCOMPINS 			0xDA

#define SSD1306_SETVCOMDETECT 		0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 	0xD5
#define SSD1306_SETPRECHARGE 		0xD9

#define SSD1306_SETMULTIPLEX 		0xA8

#define SSD1306_SETLOWCOLUMN 		0x00
#define SSD1306_SETHIGHCOLUMN 		0x10

#define SSD1306_SETSTARTLINE 		0x40

#define SSD1306_MEMORYMODE 			0x20

#define SSD1306_COMSCANINC 			0xC0
#define SSD1306_COMSCANDEC 			0xC8

#define SSD1306_SEGREMAP 			0xA0

#define SSD1306_CHARGEPUMP 			0x8D

#define SSD1306_EXTERNALVCC 		0x1
#define SSD1306_SWITCHCAPVCC 		0x2

void ssd1306PinSetup(void);
void ssd1306SendCommand(uint8_t c);
void ssd1306SendData(uint8_t c);
void ssd1306Init(void);
void ssd1306Display(void);

void Set_Start_Column(unsigned char d);
void Set_Addressing_Mode(unsigned char d);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Page_Address(unsigned char a, unsigned char b);
void Set_Start_Line(unsigned char d);
void Set_Contrast_Control(unsigned char d);
void Set_Charge_Pump(unsigned char d);
void Set_Segment_Remap(unsigned char d);
void Set_Entire_Display(unsigned char d);
void Set_Inverse_Display(unsigned char d);
void Set_Multiplex_Ratio(unsigned char d);
void Set_Display_On_Off(unsigned char d);
void Set_Start_Page(unsigned char d);
void Set_Common_Remap(unsigned char d);
void Set_Display_Offset(unsigned char d);
void Set_Display_Clock(unsigned char d);
void Set_Precharge_Period(unsigned char d);
void Set_Common_Config(unsigned char d);
void Set_VCOMH(unsigned char d);
void Fill_RAM(unsigned char Data);
void Fill_RAM_PAGE(unsigned char page, unsigned char Data);
void Fill_RAM_FONT(unsigned char number, unsigned char column,unsigned char row,unsigned char span, unsigned char width);
void Fill_RAM_FONT_SMALL(unsigned char number, unsigned char column,unsigned char row);
void Fill_RAM_Whole(const unsigned char *Data, uint8_t page, uint8_t colon, uint8_t row);
void Fill_Pixel(const unsigned char *Data, uint8_t page_start, uint8_t page_end, uint8_t colon, uint8_t row);
void Fill_DataPixel(const unsigned char Data, uint8_t page_start,uint8_t page_end, uint8_t colon, uint8_t row);

void fillPixel(uint8_t x, uint8_t y,uint8_t invert);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint8_t clr);
void drawCircle(int16_t x0, int16_t y0, int16_t radius);
void drawCircleQuads(int16_t x0, int16_t y0, int16_t radius, uint8_t quads);
void fillCircle(int16_t x0, int16_t y0, int16_t radius);
void drawHorizontalLine(int16_t x, int16_t y, int16_t length);
void drawVerticalLine(int16_t x, int16_t y, int16_t length);
void drawRect(int16_t x, int16_t y, int16_t width, int16_t height);
void fillRect(int16_t xMove, int16_t yMove, int16_t width, int16_t height);
void verticalLine(uint8_t x0,uint8_t y0, uint8_t length);
void horizontalLine(uint8_t x0,uint8_t y0, uint8_t length);void horizontalLine(uint8_t x0,uint8_t y0, uint8_t length);
#endif
