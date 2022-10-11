#ifndef _LCD1602A_H_
#define _LCD1602A_H_

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "ds1302.h"

//D0 - D7使用的是
//PB6 PB5 PB4 PB3      PA15 PA12 PA11 PA10
// RS PB9    RW PB8     E PB7
//引脚定义
#define D0    GPIO_PIN_6
#define D1    GPIO_PIN_5
#define D2    GPIO_PIN_4
#define D3    GPIO_PIN_3

#define D4    GPIO_PIN_15
#define D5    GPIO_PIN_12
#define D6    GPIO_PIN_11
#define D7    GPIO_PIN_10

#define D0_1    HAL_GPIO_WritePin(GPIOB,D0,GPIO_PIN_SET)
#define D0_0    HAL_GPIO_WritePin(GPIOB,D0,GPIO_PIN_RESET)
#define D1_1    HAL_GPIO_WritePin(GPIOB,D1,GPIO_PIN_SET)
#define D1_0    HAL_GPIO_WritePin(GPIOB,D1,GPIO_PIN_RESET)	
#define D2_1    HAL_GPIO_WritePin(GPIOB,D2,GPIO_PIN_SET)
#define D2_0    HAL_GPIO_WritePin(GPIOB,D2,GPIO_PIN_RESET)	
#define D3_1    HAL_GPIO_WritePin(GPIOB,D3,GPIO_PIN_SET)
#define D3_0    HAL_GPIO_WritePin(GPIOB,D3,GPIO_PIN_RESET)


#define D4_1    HAL_GPIO_WritePin(GPIOA,D4,GPIO_PIN_SET)
#define D4_0    HAL_GPIO_WritePin(GPIOA,D4,GPIO_PIN_RESET)	
#define D5_1    HAL_GPIO_WritePin(GPIOA,D5,GPIO_PIN_SET)
#define D5_0    HAL_GPIO_WritePin(GPIOA,D5,GPIO_PIN_RESET)	
#define D6_1    HAL_GPIO_WritePin(GPIOA,D6,GPIO_PIN_SET)
#define D6_0    HAL_GPIO_WritePin(GPIOA,D6,GPIO_PIN_RESET)	
#define D7_1    HAL_GPIO_WritePin(GPIOA,D7,GPIO_PIN_SET)
#define D7_0    HAL_GPIO_WritePin(GPIOA,D7,GPIO_PIN_RESET)

//PD4 需要检测是否busy
#define Read_Busy  HAL_GPIO_ReadPin(GPIOA,D7)  


#define LCD_RS	  GPIO_PIN_9
#define LCD_R_W   GPIO_PIN_8
#define LCD_E     GPIO_PIN_7

#define LCD_RS_1    HAL_GPIO_WritePin(GPIOB,LCD_RS,GPIO_PIN_SET)
#define LCD_RS_0    HAL_GPIO_WritePin(GPIOB,LCD_RS,GPIO_PIN_RESET)
#define LCD_R_W_1   HAL_GPIO_WritePin(GPIOB,LCD_R_W,GPIO_PIN_SET)	
#define LCD_R_W_0   HAL_GPIO_WritePin(GPIOB,LCD_R_W,GPIO_PIN_RESET)
#define LCD_E_1   	HAL_GPIO_WritePin(GPIOB,LCD_E,GPIO_PIN_SET)	
#define LCD_E_0  	  HAL_GPIO_WritePin(GPIOB,LCD_E,GPIO_PIN_RESET)	

// -----------------函数声明----------------

void IO_Init(void);
unsigned char ReadStatusLCD(void);
void WriteInput(unsigned char WDLCD);
unsigned char ReadStatusLCD(void);
/*写数据*/
void WriteDataLCD(unsigned char WDLCD);  
void WriteCommandLCD(unsigned char WCLCD,int BuysC); //BuysC为0时忽略忙检测
void LCDInit(void); //LCM初始化
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
//void show_time(void);
void Show_T(void);
void LCD_Clear(void);  //LCD屏幕清屏
#endif