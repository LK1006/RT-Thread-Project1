/*
 * File:        nokia.h
 * Purpose:     Serial Input/Output routines
 *
 */

#ifndef _OLED_H
#define _OLED_H

#include "stm32f1xx_hal.h"
/********************************************************************/
/*-----------------------------------------------------------------------
LCD_init          : OLED初始化

编写日期          ：2012-11-01
最后修改日期      ：2012-11-01
-----------------------------------------------------------------------*/
//#define u8 uint8_t
//#define u16 uint16_t

 extern uint8_t lanzhou96x64[768];
 void OLED_Init(void);
 void OLED_CLS(void);
 void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_P8x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
 void OLED_Print(uint8_t x, uint8_t y, uint8_t ch[]);
 void OLED_PutPixel(uint8_t x,uint8_t y);
 void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
 void OLED_Set_Pos(uint8_t x, uint8_t y);
 void OLED_WrDat(uint8_t data);
 void Draw_LibLogo(void);
 void Draw_Landzo(void);
 void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]);
 void OLED_Fill(uint8_t dat);
 void Dly_ms(uint16_t ms);
 void xianshi(float abc,int x,int y);
 void OLED_PrintU16(unsigned char x,unsigned char y,unsigned int num);
 uint32_t OLED_Pow(uint8_t m,uint8_t n);
 void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,uint8_t len);
 void OLED_PrintU16_2(unsigned char x,unsigned char y,unsigned int num);
 void OLED_PrintU16_3(unsigned char x,unsigned char y,unsigned int num);
 void OLED_PrintFloat(unsigned char x,unsigned char y,float num);
/********************************************************************/
 void draw_line(uint8_t x,uint8_t y,uint8_t deltax,uint8_t deltay);
 void draw_num(uint8_t x, uint8_t y,uint8_t num);
 
 void OLED_Print2(unsigned char x, unsigned char y, unsigned char mode,unsigned char ch[]);
#endif
