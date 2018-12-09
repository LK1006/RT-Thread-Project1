#ifndef _LEDMATRIX_H_
#define _LEDMATRIX_H_
//#include "board.h"
#include <stdlib.h>
#include <stdint.h>
#include "stm32f1xx_hal.h"

//#define OE_LED_DIS       GPIOA->BSRR = GPIO_PIN_0;
//#define OE_LED_EN        GPIOA->BSRR = (GPIO_PIN_0 << 16);

#define LINE_1 38   //16，0使数字显示在最低部
#define LINE_2 8		//38，8使数字显示在中间

#define MOVE_BUFF 22

#define LED_BUFF_SIZE   768


//红 绿 蓝 黄 紫 青 白
typedef enum
{
    RED = 0,
    GREEN,
    BLUE,
    YELLOW,
    PURPLE,
    CYAN,
    WHITE,
    BLACK
}ColorDef;
extern ColorDef m_color;
extern ColorDef dot_color;
#pragma anon_unions
/*
   二维数组指针*(*(a+i)+j)
 */
typedef struct _dat_rgb_t
{
    union
    {
        uint8_t r[32][8];
        uint32_t r32[32][2];
    };

    union
    {
        uint8_t g[32][8];
        uint32_t g32[32][2];
    };

    union
    {
        uint8_t b[32][8];
        uint32_t b32[32][2];
    };
    //2000年01月01日 星期三       //21 24 /8 3
    uint8_t move_buf[16][MOVE_BUFF];  //display buff for move
    ColorDef color;    // 0~6 红 绿 蓝 黄 紫 青 白
}dat_rgb_t;
extern dat_rgb_t dat;

typedef struct _disp_time_t
{
    uint8_t year;
    uint8_t mon;
    uint8_t day;
    uint8_t week;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
}disp_time_t;
extern disp_time_t disp_t;

void GPIO_init(void);
void draw_pixel(uint8_t x, uint8_t y,ColorDef c);
void draw_line_rgb(uint8_t x, uint8_t y,uint8_t deltax, uint8_t deltay, ColorDef c);
void clean_line(uint8_t x, uint8_t y,uint8_t deltax, uint8_t deltay, ColorDef c);
void Effects_one(void);
void Effects_two(void);
void move_effect(ColorDef color);
void led_scan(void);
void color_init(void);
void color_change(void);
ColorDef get_color(void);
void disp_background(uint8_t index,ColorDef c);
void Lighting_LED(ColorDef c);
#endif

