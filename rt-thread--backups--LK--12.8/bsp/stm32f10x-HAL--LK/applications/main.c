/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include "led.h"
#include "oled.h"
#include "drv_rtc.h"
#include "timer.h"
#include "ledmatrix.h"
#include "morphdigit.h"
#include "OLED_Display.h"

static rt_thread_t led1_thread = RT_NULL;
static rt_thread_t rgbled_thread = RT_NULL;

static void led1_thread_entry(void* parameter);
static void rgbled_thread_entry(void* parameter);


/******* 变量定义 *******/
static struct tm *tmp;
time_t now;
disp_time_t disp_t;

uint8_t Menu_ON;
uint8_t RGBLED_ON = 1;
int count_oled;
int count_rgbled;
int count_dis;
int Lightness_Value = 100;
uint8_t Lightness_flag;
uint8_t Lightness_Change_finish;
/******* 主函数 ******/
int main(void)
{
	int i;
	RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;
	HAL_Init();
	LED_Init();
	OLED_Init();
//	OLED();
	TIM3_Init(7,7199);       //定时器3初始化，定时器时钟为72M，分频系数为7200-1，
                             //所以定时器3的频率为72M/7200=10K，自动重装载为5000-1，那么定时器周期就是500ms
	TIM4_Init(4999,7199);
	TIM2_PWM_Init(150-1,3-1);      //90M/90=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	TIM_SetTIM2Compare1(80);      //20:85%   1:98%    148:0%    //值越大越亮
	GPIO_init();               //网络时钟端口初始化
	rt_hw_rtc_wakeup_enable();
	  for(i=0;i<7;i++)
    {
        disp_background(9,i);
        rt_thread_delay(250);
    }
    disp_background(0,0);
    /* user app entry */
//	led1_thread = 
//		rt_thread_create("led1",
//											led1_thread_entry,
//											RT_NULL,
//											512,
//											2,
//											2);
//	if(led1_thread != NULL)
//	  rt_thread_startup(led1_thread);
//	else
//		return 1;
	
	rgbled_thread = 
		rt_thread_create("rgbled",
										 rgbled_thread_entry,
	                   RT_NULL,
										 512,
										 1,
                     2);	
	if(rgbled_thread != NULL)
		rt_thread_startup(rgbled_thread);
	else
		return 1;
	
}



static void led1_thread_entry(void* parameter)
{
	while(1)
	{
//	LED2(0);
//	rt_thread_delay(100);   /* 延时500个tick */
//	LED2(1);
		rt_thread_delay(30);   /* 延时500个tick */
	}
}

static void rgbled_thread_entry(void* parameter)
{
	while(1)
	{
		key_select();
		if(KEY_X == 0)  //up
		{			
			rt_thread_delay(100);

			if(Lightness_flag == 1)
			{
				Lightness_Change_finish = 1;
				Lightness_Value+=3;
				if(Lightness_Value>147)
				{
					Lightness_Value = 148;
				}
				TIM_SetTIM2Compare1(Lightness_Value);
			}
			else
			{
				count_rgbled++;
				if(count_rgbled%2==1)
				{
					Effects_one();
					rt_thread_delay(50);
					RGBLED_ON = 0;
				}
				if(count_rgbled%2==0)
				{
					count_rgbled = 0;
					RGBLED_ON = 1;				
	//				Effects_two();
	//				rt_thread_delay(20);
					
					/**** 获取时间 ****/		
					now = time(RT_NULL);
					tmp = localtime(&now);
					disp_t.hour = tmp->tm_hour;
					disp_t.min = tmp->tm_min;
					disp_t.sec = tmp->tm_sec;
					morph_init(&disp_t);			
					disp_background(1,1);				
				}	
			}			
		}
		if(KEY_Y == 0)  //down
		{
			rt_thread_delay(150);
			
			if(Lightness_flag == 1)
			{
				Lightness_Change_finish = 1;
				Lightness_Value-=3;
				if(Lightness_Value<4)
				{
					Lightness_Value = 2;
				}
				TIM_SetTIM2Compare1(Lightness_Value);
			}
			
			else
			{
				count_oled++;
				if(count_oled%2==1)
				{
					Menu_ON = 1;
					OLED();
					
					
					Effects_one();
					rt_thread_delay(50);
					RGBLED_ON = 0;
				}
				if(count_oled%2==0)
				{
					OLED_Fill(0x00);
					Menu_ON = 0;
					count_oled = 0;
					
					RGBLED_ON = 1;
					morph_init(&disp_t);			
					disp_background(1,1);						
				}
		}			
		}
		
		if(RGBLED_ON == 1 && Light_ON != 1)
		{
//		LED1(0);
//		LED2(1);
//		rt_thread_delay(100);   /* 延时500个tick */
////		LED1(1);
//		LED2(0);
//		rt_thread_delay(100);   /* 延时500个tick */
			now = time(RT_NULL);
			tmp = localtime(&now);
		/**** 时间动态显示 ****/
			disp_t.hour = tmp->tm_hour;
			disp_t.min = tmp->tm_min;
			disp_t.sec = tmp->tm_sec;		
			if(Change_Color == 1)
			{
				morph_update(&disp_t,(ColorDef)get_color());
			}
			else 
			{
				morph_update(&disp_t,m_color);
			}
			if(disp_t.hour == 23 && disp_t.min == 29)
			{
				TIM_SetTIM2Compare1(1);
			}
			else if(disp_t.hour == 6 && disp_t.min == 40)
			{
				TIM_SetTIM2Compare1(90);
			}
	//		draw_line_rgb(0,32,64,0,(ColorDef)get_color());
	//		Lighting_LED(6);
	//			draw_line_rgb(0,28,64,0,6);
		}
		if(Menu_ON == 1)
		{
			OLED_switch();
//			key_select();
		}
		if(Light_ON == 1 && count_dis <2)
		{
			if(Menu_ON == 0)
				count_dis++;
			if(count_dis>=4)
			{
				count_dis = 4;
			}
			if(Menu_ON == 0)
			{
				Effects_one();
				disp_background(7,m_color);
			}
		}
	}
}
