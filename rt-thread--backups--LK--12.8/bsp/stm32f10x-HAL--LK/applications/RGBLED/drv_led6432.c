/*
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-22     YHX      first implementation
 *
 */
#include <rtdevice.h>
#include "board.h"
#include "drv_led6432.h"

#define LED_R1  29      //B10
#define LED_G1  28      //B2
#define LED_B1  27      //B1
#define LED_R2  26      //B0
#define LED_G2  25      //C5
#define LED_B2  24      //C4
#define LED_A   11      //C3
#define LED_B   10      //C2
#define LED_C   9       //C1
#define LED_D   8       //C0
#define PIN_CLK 16      //A2
#define PIN_LAT 15      //A1
#define PIN_OEN 14      //A0

#define OEN_DIS     (GPIOA->BSRR = GPIO_PIN_0)
#define OEN_EN      (GPIOA->BSRR = GPIO_PIN_0 << 16)
#define LAT_UNLOCK  (GPIOA->BSRR = GPIO_PIN_1)
#define LAT_LOCK    (GPIOA->BSRR = GPIO_PIN_1 << 16)
#define CLK_H       (GPIOA->BSRR = GPIO_PIN_2)
#define CLK_L       (GPIOA->BSRR = GPIO_PIN_2 << 16)
#define DAT_R1(x)    ((x)?(GPIOB->BSRR = GPIO_PIN_10):(GPIOB->BSRR = GPIO_PIN_10 << 16))
#define DAT_G1(x)    ((x)?(GPIOB->BSRR = GPIO_PIN_2):(GPIOB->BSRR = GPIO_PIN_2 << 16))
#define DAT_B1(x)    ((x)?(GPIOB->BSRR = GPIO_PIN_1):(GPIOB->BSRR = GPIO_PIN_1 << 16))
#define DAT_R2(x)    ((x)?(GPIOB->BSRR = GPIO_PIN_0):(GPIOB->BSRR = GPIO_PIN_0 << 16))
#define DAT_G2(x)    ((x)?(GPIOC->BSRR = GPIO_PIN_5):(GPIOC->BSRR = GPIO_PIN_5 << 16))
#define DAT_B2(x)    ((x)?(GPIOC->BSRR = GPIO_PIN_4):(GPIOC->BSRR = GPIO_PIN_4 << 16))

uint8_t rgb_ram[RGB_BUFF_SIZE];
uint8_t move_ram[MOVE_BUFF_SIZE];
rt_sem_t sem_move;
uint8_t g_color;
rt_bool_t blink = 1;

/*
 * x=0~63 y=0~31
 */
void draw_pixel(uint8_t x, uint8_t y,uint8_t color)
{
    if ((x > 63) || (y > 31))
        return;

	rgb_ram[64 * y + x] = color;
}

/*
 * deltax > x, deltay > y
 */
void draw_line(uint8_t x, uint8_t y,uint8_t deltax, uint8_t deltay, uint8_t color)
{
    static uint8_t i;
    if (deltax == 0)
    {
        for (i = 0; i < deltay; i++)
        {
            draw_pixel(x, y + i, color);
        }
    }
    else if (deltay == 0)
    {
        for (i = 0; i < deltax; i++)
        {
            draw_pixel(x + i, y, color);
        }
    }
}

void fill_move_ram(uint8_t x, uint8_t *pVal)
{
    uint8_t i;
    for (i = 0; i < MOVE_HEIGHT; i++)
    {
        move_ram[i * MOVE_LENGTH + x] = *(pVal + i);
    }
}

void single2rgb332(uint8_t row,  uint8_t line, uint8_t color)
{
    static uint8_t i, j;
    static uint8_t tmp,bit;
    static uint16_t move_ram_pos;

    move_ram_pos = row * MOVE_LENGTH;
    bit = move_ram[move_ram_pos] & 0x1;
    for (i = 0; i < MOVE_LENGTH; i++)
    {
        if (i != MOVE_LENGTH - 1)
        {
            move_ram[move_ram_pos + i] >>= 1;
            if (move_ram[move_ram_pos + i + 1] & 0x1)
            {
                move_ram[move_ram_pos + i] |= 0x80;
            }
        }
        else
        {
            move_ram[move_ram_pos + i] >>= 1;
            if (bit)
            {
                move_ram[move_ram_pos + i] |= 0x80;
            }
        }

        if (i < 8)
        {
            tmp = move_ram[move_ram_pos + i];
            for (j = 0; j < 8; j++)
            {
                if (tmp & 0x1)
                {
                    rgb_ram[((row + line) * 64) + (i * 8) + j] = color;
                }
                else
                {
                    rgb_ram[((row + line) * 64) + (i * 8) + j] = BLACK;
                }
                tmp >>= 1;
            }
        }
    }
}

void move_effect(uint8_t line, uint8_t color)
{
    static uint8_t i;

    rt_sem_take(sem_move, RT_WAITING_FOREVER);
	for (i = 0; i < MOVE_HEIGHT; i++)
	{
        single2rgb332(i, line, color);
	}
    rt_sem_release(sem_move);
}

static void shift_dat_gray_level(uint8_t row, uint8_t level)
{
    static uint8_t i;
    static uint8_t red1,red2;
    static uint8_t green1,green2;
    static uint8_t blue1,blue2;
	static uint8_t rgb_data1,rgb_data2;

	uint32_t row_1 = row * 64;
	uint32_t row_2 = (row + 16) * 64;

	for (i = 0; i < 64; i++)
	{
		rgb_data1 = rgb_ram[row_1 + i];
		rgb_data2 = rgb_ram[row_2 + i];

        red1 = (rgb_data1 >> 5) & 0x7;
        red2 = (rgb_data2 >> 5) & 0x7;

        green1 = (rgb_data1 >> 2) & 0x7;
        green2 = (rgb_data2 >> 2) & 0x7;

        blue1 = rgb_data1 & 0x3;
        blue2 = rgb_data2 & 0x3;

        CLK_L;
        DAT_R1(red1 > level);
        DAT_R2(red2 > level);
        DAT_G1(green1 > level);
        DAT_G2(green2 > level);
        DAT_B1(blue1 > level);
        DAT_B2(blue2 > level);
        CLK_H;
	}
    CLK_L;
}

static uint8_t swap_half_byte(uint8_t byte)
{
	byte = (byte << 2) | (byte >> 2);
    byte = ((byte << 1) & 0xa) | ((byte >> 1) & 0x5);
    return byte;
}

void led_blink(uint8_t sw)
{
    if (sw)
    {
        blink = 0;
    }
    else
    {
        blink = 1;
    }
}

void led_en(uint8_t en)
{
    if (en && blink)
    {
        OEN_EN;
    }
    else
    {
        OEN_DIS;
    }
}

void led_scan(void)
{
    static uint8_t index = 0;
    static uint8_t row = 0;
    static uint8_t scan_cnt = 0;

    rt_sem_take(sem_move, RT_WAITING_FOREVER);
    shift_dat_gray_level(index,scan_cnt);    // 40us -> 60us when 84M
    rt_sem_release(sem_move);
    row = swap_half_byte(index);            // 200ns when 84M
    LAT_UNLOCK;
    GPIOC->BSRR = (((~row) & 0xf) << 16) | row; // 100ns when 84M
    LAT_LOCK;

    if (index++ >= 0xf)
    {
        index = 0;
        if (scan_cnt++ > 0x7)
        {
            scan_cnt = 0;
        }
    }
}

int rt_hw_led6432_init(void)
{
    /* creat sem */
    sem_move = rt_sem_create("s_move", 1, RT_IPC_FLAG_FIFO);
    RT_ASSERT(sem_move != RT_NULL);

    rt_pin_mode(LED_R1, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_G1, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_B1, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_R2, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_G2, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_B2, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_A,  PIN_MODE_OUTPUT);
    rt_pin_mode(LED_B,  PIN_MODE_OUTPUT);
    rt_pin_mode(LED_C,  PIN_MODE_OUTPUT);
    rt_pin_mode(LED_D,  PIN_MODE_OUTPUT);
    rt_pin_mode(PIN_CLK, PIN_MODE_OUTPUT);
    rt_pin_mode(PIN_LAT, PIN_MODE_OUTPUT);
    rt_pin_mode(PIN_OEN, PIN_MODE_OUTPUT);
    return 0;
}
INIT_DEVICE_EXPORT(rt_hw_led6432_init);
