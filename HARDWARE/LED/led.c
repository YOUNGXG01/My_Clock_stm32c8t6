#include "led.h"
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

//初始化PB1为输出.并使能时钟	    
//LED IO初始化

//使用PC13
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

	
    __HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_SWJ_NOJTAG();
    __HAL_RCC_GPIOC_CLK_ENABLE();           	//开启GPIOB时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOB时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟

//	  GPIO_Initure.Pin=GPIO_PIN_4; 				//PB5
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
//    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    GPIO_Initure.Pin=GPIO_PIN_13 ; 				//PB5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
//	    GPIO_Initure.Pin= GPIO_PIN_11 ; 				//PB5
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
//    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
//		
//	    GPIO_Initure.Pin= GPIO_PIN_15 ; 				//PB5
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
//    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
}

//#define Add_PIN  GPIO_PIN_15
//#define Dec_PIN  GPIO_PIN_12
//#define Alarm_PIN  GPIO_PIN_14
//#define Time_PIN  GPIO_PIN_11
//#define Watch_PIN  GPIO_PIN_15  //秒表
//#define Key_PIN  GPIO_PIN_15

void KEY_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
			//先改成输入进行检测
		GPIO_Initure.Pin=Add_PIN | Dec_PIN | Alarm_PIN| Time_PIN |Watch_PIN | Key_PIN; 
		GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
		GPIO_Initure.Pull=GPIO_PULLDOWN;          //下拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!



u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(Add_Key==1||Dec_Key==1||Alarm_Key==1 || Time_Key == 1|| Watch_Key == 1 || KEY == 1))
    {
        delay_ms(10);
        key_up=0;
        if(Add_Key==1)       return Add_PRES;
        else if(Dec_Key==1)  return Dec_PRES;
        else if(Alarm_Key==1) return Alarm_PRES;   
				else if(Time_Key==1)  return Time_PRES;
        else if(Watch_Key==1) return Watch_PRES;
				else if(KEY==1) return KEY_PRES; 			
    }
		else if(Add_Key==0&&Dec_Key==0&&Alarm_Key==0&&Time_Key==0&&Watch_Key==0&&KEY==0)key_up=1;
    return 0;   //无按键按下
}


// Add_PRES  //Add_Key  
// Dec_PRES  //Dec_Key  
// Alarm_PRES//Alarm_Key
// Time_PRES //Time_Key 
// Watch_PRES//Watch_Key
// KEY_PRES  //KEY      