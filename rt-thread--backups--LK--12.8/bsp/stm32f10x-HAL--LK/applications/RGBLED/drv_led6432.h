/*
 * Date           Author       Notes
 * 2018-11-22     YHX      first implementation
 */

#ifndef __DRV_LED6432_H__
#define __DRV_LED6432_H__
#include "board.h"

#define RGB_BUFF_SIZE (64 * 32)
#define MOVE_LENGTH     24
#define MOVE_HEIGHT     16
#define MOVE_BUFF_SIZE (MOVE_LENGTH * MOVE_HEIGHT)

#define RED 0xE0        // 1110 0000
#define GREEN 0x1C      // 0001 1100
#define BLUE 0x3        // 0000 0011
#define WHITE 0xFF      // 1111 1111
#define BLACK 0x00      // 0000 0000

#define YELLOW  (RED | GREEN)
#define PURPLE  (RED | BLUE)
#define CYAN    (GREEN | BLUE)


extern uint8_t rgb_ram[RGB_BUFF_SIZE];      // for RGB332
extern uint8_t move_ram[MOVE_BUFF_SIZE];    // for single color, display buff for move
extern rt_sem_t sem_move;

extern uint8_t g_color;

void draw_pixel(uint8_t x, uint8_t y,uint8_t color);
void draw_line(uint8_t x, uint8_t y,uint8_t deltax, uint8_t deltay, uint8_t color);

void led_scan(void);
void led_en(uint8_t en);
void led_blink(uint8_t sw);
void move_effect(uint8_t line, uint8_t color);
void fill_move_ram(uint8_t x, uint8_t *pVal);

#endif
