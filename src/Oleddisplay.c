/*
 * Oleddisplay.c
 *
 *  Created on: 04-May-2017
 *      Author: hardik
 */
/*
 * OLED_Display.c
 *
 *  Created on: 06-Oct-2016
 *      Author: hardik.soni
 */

#include <stdint.h>
#include "Oleddisplay.h"
#include "SSD1306.h"
//#include "Font_14x15.h"
//#include "Font_5x7.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static unsigned char row = 0;
static unsigned char columnIncrementSmallFont = 0;
static unsigned char columnIncrementLargeFont = 0;
static enum e_columnIncrementFlag columnIncrementFlag = NO_FONT;


static void putChar(char character)
{
   if (columnIncrementFlag == SMALL_FONT)
   {
   	Fill_RAM_FONT_SMALL((unsigned char)(character-32),columnIncrementSmallFont,row);
   	columnIncrementSmallFont = (unsigned char)(columnIncrementSmallFont + SMALL_FONT_WIDTH);
   }

   if (columnIncrementFlag == LARGE_FONT)
   {
   	Fill_RAM_FONT((unsigned char)character,columnIncrementLargeFont,row,LARGE_FONT_SPAN,LARGE_FONT_WIDTH);
   	columnIncrementLargeFont = (unsigned char)(columnIncrementLargeFont+LARGE_FONT_WIDTH);
   }
}


static void putString(char *s)
{
  while(*s)
  {
    if (columnIncrementFlag == SMALL_FONT)
    {
      Fill_RAM_FONT_SMALL((unsigned char)(*s-32),columnIncrementSmallFont,row);
      columnIncrementSmallFont = (unsigned char)(columnIncrementSmallFont+SMALL_FONT_WIDTH);
    }

    if (columnIncrementFlag == LARGE_FONT)
    {
      Fill_RAM_FONT((unsigned char)*s,columnIncrementLargeFont,row,LARGE_FONT_SPAN,LARGE_FONT_WIDTH);
      columnIncrementLargeFont = (unsigned char)(columnIncrementLargeFont+LARGE_FONT_WIDTH);
    }
    s++;
  }
}

void OLED_PrintfSetColumnRow(char setColumn, char setRow, char IncrementFlag)
{
	row = (unsigned char)setRow;
	columnIncrementFlag = (enum e_columnIncrementFlag)IncrementFlag;

	if (columnIncrementFlag == SMALL_FONT)
		columnIncrementSmallFont = (unsigned char)setColumn;

	if (columnIncrementFlag == LARGE_FONT)
		columnIncrementLargeFont = (unsigned char)setColumn;
}

static const unsigned long dv[] = {
//  4294967296      // 32 bit unsigned max
    1000000000,     // +0
     100000000,     // +1
      10000000,     // +2
       1000000,     // +3
        100000,     // +4
//       65535      // 16 bit unsigned max
         10000,     // +5
          1000,     // +6
           100,     // +7
            10,     // +8
             1,     // +9
};

static void xtoa(unsigned long x, const unsigned long *dp)
{
    char c;
    unsigned long d;
    if(x) {
        while(x < *dp) ++dp;
        do {
            d = *(dp++);
            c = '0';
            while(x >= d) ++c, x -= d;
            putChar(c);
        } while(!(d & 1));
    } else
        putChar('0');
}

static void putHex(int n)
{
    static const char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    putChar(hex[n & 15]);
}

void OLED_Printf(char *format, ...)
{
    char c;
    int i;
    long n;

    va_list a;
    va_start(a, format);
    while((c = *format++))
    //c = *format++;
    {
        if(c == '%') {
            switch(c = *format++) {
                case 's':                       // String
                    putString(va_arg(a, char*));
                    break;
//                case 'c':                       // Char
//                    putChar(va_arg(a, char));
//                    break;
                case 'i':                       // 16 bit Integer
                case 'd':                       // 16 bit Integer
                case 'u':                       // 16 bit Unsigned
                    i = va_arg(a, int);
                    if(c == 'i' && i < 0) i = -i, putChar('-');
                    xtoa((unsigned)i, dv + 5);
                    break;
                case 'l':                       // 32 bit Long
                case 'n':                       // 32 bit uNsigned loNg
                    n = va_arg(a, long);
                    if(c == 'l' &&  n < 0) n = -n, putChar('-');
                    xtoa((unsigned long)n, dv);
                    break;
                case 'x':                       // 16 bit heXadecimal
                    i = va_arg(a, int);
                    putHex(i >> 12);
                    putHex(i >> 8);
                    putHex(i >> 4);
                    putHex(i);
                    break;
                case 0: return;
                default: goto bad_fmt;
            }
        } else
        {
				bad_fmt:    putChar(c);

        }
    }
    va_end(a);
}
