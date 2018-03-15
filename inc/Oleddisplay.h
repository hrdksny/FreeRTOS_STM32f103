/*
 * Oleddisplay.h
 *
 *  Created on: 04-May-2017
 *      Author: hardik
 */

#ifndef OLEDDISPLAY_H_
#define OLEDDISPLAY_H_

enum e_columnIncrementFlag { NO_FONT, SMALL_FONT, LARGE_FONT };
typedef enum e_columnIncrementFlag FontWidth_t;

#define LARGE_FONT_WIDTH	14
#define LARGE_FONT_HEIGHT	15
#define LARGE_FONT_SPAN		(LARGE_FONT_HEIGHT/8)+1

#define SMALL_FONT_WIDTH	5


void OLED_Printf(char *format, ...);
void OLED_PrintfSetColumnRow(char setColumn, char setRow, char IncrementFlag);


#endif /* OLEDDISPLAY_H_ */
