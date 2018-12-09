#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOB时钟
	__HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOB时钟
	
  GPIO_Initure.Pin=GPIO_PIN_2; //PB1,0
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;     //高速
  HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_12;
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);	//PB0置1，默认初始化后灯灭
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);	//PB1置1，默认初始化后灯灭
}

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOC_CLK_ENABLE();  
	
	GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11; 
	GPIO_Initure.Mode=GPIO_MODE_INPUT;
	GPIO_Initure.Pull=GPIO_PULLUP;         
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
	GPIO_Initure.Pin=GPIO_PIN_5;
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_10,GPIO_PIN_SET);	
}

void menu_rgbled(void)
{
	
}