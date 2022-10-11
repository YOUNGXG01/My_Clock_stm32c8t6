#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/9/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define LEDC PCout(5)   	//LED0


#define Add_PIN  GPIO_PIN_10
#define Dec_PIN  GPIO_PIN_12
#define Alarm_PIN  GPIO_PIN_14
#define Time_PIN  GPIO_PIN_11
#define Watch_PIN  GPIO_PIN_1  //���
#define Key_PIN  GPIO_PIN_13


#define Add_Key    HAL_GPIO_ReadPin(GPIOB,Add_PIN)  //KEY0����PE4
#define Dec_Key    HAL_GPIO_ReadPin(GPIOB,Dec_PIN)  //KEY0����PE4
#define Alarm_Key  HAL_GPIO_ReadPin(GPIOB,Alarm_PIN)  //KEY0����PE4
#define Time_Key   HAL_GPIO_ReadPin(GPIOB,Time_PIN)  //KEY0����PE4
#define Watch_Key  HAL_GPIO_ReadPin(GPIOB,Watch_PIN)  //KEY0����PE4
#define KEY        HAL_GPIO_ReadPin(GPIOB,Key_PIN)  //KEY0����PE4

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
