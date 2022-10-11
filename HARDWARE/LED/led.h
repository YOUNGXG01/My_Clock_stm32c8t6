#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/9/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define LEDC PCout(5)   	//LED0


#define Add_PIN  GPIO_PIN_10
#define Dec_PIN  GPIO_PIN_12
#define Alarm_PIN  GPIO_PIN_14
#define Time_PIN  GPIO_PIN_11
#define Watch_PIN  GPIO_PIN_1  //秒表
#define Key_PIN  GPIO_PIN_13


#define Add_Key    HAL_GPIO_ReadPin(GPIOB,Add_PIN)  //KEY0按键PE4
#define Dec_Key    HAL_GPIO_ReadPin(GPIOB,Dec_PIN)  //KEY0按键PE4
#define Alarm_Key  HAL_GPIO_ReadPin(GPIOB,Alarm_PIN)  //KEY0按键PE4
#define Time_Key   HAL_GPIO_ReadPin(GPIOB,Time_PIN)  //KEY0按键PE4
#define Watch_Key  HAL_GPIO_ReadPin(GPIOB,Watch_PIN)  //KEY0按键PE4
#define KEY        HAL_GPIO_ReadPin(GPIOB,Key_PIN)  //KEY0按键PE4

#define Add_PRES  1
#define Dec_PRES  2
#define Alarm_PRES  3
#define Time_PRES  4
#define Watch_PRES  5
#define KEY_PRES  6


void LED_Init(void);

void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
