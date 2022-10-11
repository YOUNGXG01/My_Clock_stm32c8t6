#ifndef _SD18B20_H_
#define _SD18B20_H_
#include "sys.h"
#include "delay.h"

extern unsigned char temp;
extern unsigned int sdata_t;

#define DQ 	  GPIO_PIN_9
#define DQ_1  HAL_GPIO_WritePin(GPIOA,DQ,GPIO_PIN_SET)
#define DQ_0  HAL_GPIO_WritePin(GPIOA,DQ,GPIO_PIN_RESET)



#define Read_DQ  HAL_GPIO_ReadPin(GPIOA,DQ)  



void DS18B20_Init_Out(void);
void DS18B20_Init_In(void);
void DS18B20_Rst(void);
unsigned char DS18B20_Read_Bit(void);
unsigned char DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(u8 dat);
u8 DS18B20_Check(void);
void DS18B20_Start(void);
short DS18B20_Get_Temp(void);



#endif