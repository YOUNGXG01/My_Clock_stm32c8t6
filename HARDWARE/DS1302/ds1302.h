#ifndef _DS1302_H_
#define _DS1302_H_

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f1xx_hal.h"

// data PA8  rst PB10  CLK PB15
 typedef unsigned char uchar;
 
 
 // 枚举体定义双向 IO
typedef enum {
	DATA_IN,
	DATA_OUT,
}DS_IO;

extern DS_IO ds_io;
extern uchar label2[16];//时间存放数组


#define DS_CLK   GPIO_PIN_15
#define DS_RST   GPIO_PIN_10
#define DS_DATA  GPIO_PIN_8


#define RES_Set_1  HAL_GPIO_WritePin(GPIOB,DS_RST,GPIO_PIN_SET)
#define RES_Set_0  HAL_GPIO_WritePin(GPIOB,DS_RST,GPIO_PIN_RESET)

#define DS_CLK_1  HAL_GPIO_WritePin(GPIOB,DS_CLK,GPIO_PIN_SET)
#define DS_CLK_0  HAL_GPIO_WritePin(GPIOB,DS_CLK,GPIO_PIN_RESET)

#define DS_DATA_1  HAL_GPIO_WritePin(GPIOA,DS_DATA,GPIO_PIN_SET)
#define DS_DATA_0  HAL_GPIO_WritePin(GPIOA,DS_DATA,GPIO_PIN_RESET)
#define IO_Read		 HAL_GPIO_ReadPin(GPIOA,DS_DATA) 

/*************************/
 #define Time_24_Hour    0x00    //24小时制
 #define Time_Start     0x00    //开始走时

//DS1302寄存器操作指令定义时间地址
 #define ds1302_sec_addr   0x80       //秒 
 #define ds1302_min_addr   0x82       //分
 #define ds1302_hour_addr  0x84       //时
 #define ds1302_day_addr   0x86       //日
 #define ds1302_month_addr 0x88       //月
 #define ds1302_year_addr  0x8c       //年 
 
 

void DS1302_init(void);
void DS1302_Data(DS_IO type_io);
void DS1302_Write(uchar add,uchar dat);
uchar DS1302_Read(uchar add);
void DS1302_SetTime(uchar *ad);
void DS1302_OFF(void);
void DS1302_ON(void);
void DS1302_Init(uchar *time);
void DS1302_Readtime(void);


#endif