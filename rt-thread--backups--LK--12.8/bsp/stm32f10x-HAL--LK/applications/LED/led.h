#ifndef _LED_H
#define _LED_H

#include "stm32f1xx_hal.h"


#define LED1(a) if(a) \
							  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);\
							 else \
							  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);

#define LED2(a) if(a) \
							  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);\
							 else \
							  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);

#define KEY_UP    HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)
#define KEY_DOWN  HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_11)	
#define KEY_L     HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)	
#define KEY_R     HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)											 
#define KEY_A     HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)

#define KEY_X     HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)
#define KEY_Y     HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)							 



void LED_Init(void);
void KEY_Init(void);
void menu_rgbled(void);							 
#endif