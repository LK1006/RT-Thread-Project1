#ifndef _MORPHDIGIT_H_
#define _MORPHDIGIT_H_

#include "ledmatrix.h"

extern disp_time_t disp_t;

extern int Light_ON;
extern int Change_Color;

void draw_pixel_offset(uint8_t x, uint8_t y, ColorDef c);
void draw_line_offset(uint8_t x, uint8_t y,uint8_t deltax, uint8_t deltay, ColorDef c);
void draw_seg(uint8_t seg);
void draw_num(uint8_t x, uint8_t y,uint8_t num);
void morph(uint8_t x,uint8_t y,uint8_t newValue);
void morph_update(disp_time_t *pdt,ColorDef c);
void morph_init(disp_time_t *pdt);
#endif

