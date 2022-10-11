#ifndef _DS1302_H_
#define _DS1302_H_

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f1xx_hal.h"

// data PA8  rst PB10  CLK PB15
 typedef unsigned char uchar;
 
 
 // ö���嶨��˫�� IO
typedef enum {
	DATA_IN,
	DATA_OUT,
}DS_IO;

extern DS_IO ds_io;
extern uchar label2[16];//ʱ��������


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
 #define Time_24_Hour    0x00    //24Сʱ��
 #define Time_Start     0x00    //��ʼ��ʱ

//DS1302�Ĵ�������ָ���ʱ���ַ
 #define ds1302_sec_addr   0x80       //�� 
 #define ds1302_min_addr   0x82       //��
 #define ds1302_hour_addr  0x84       //ʱ
 #define ds1302_day_addr   0x86       //��
 #define ds1302_month_addr 0x88       //��
 #define ds1302_year_addr  0x8c       //�� 
 
 

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