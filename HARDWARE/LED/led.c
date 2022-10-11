#include "led.h"
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

//��ʼ��PB1Ϊ���.��ʹ��ʱ��	    
//LED IO��ʼ��

//ʹ��PC13
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

	
    __HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_SWJ_NOJTAG();
    __HAL_RCC_GPIOC_CLK_ENABLE();           	//����GPIOBʱ��
		__HAL_RCC_GPIOA_CLK_ENABLE();           	//����GPIOBʱ��
		__HAL_RCC_GPIOB_CLK_ENABLE();           	//����GPIOBʱ��

//	  GPIO_Initure.Pin=GPIO_PIN_4; 				//PB5
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    GPIO_Initure.Pin=GPIO_PIN_13 ; 				//PB5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
//	    GPIO_Initure.Pin= GPIO_PIN_11 ; 				//PB5
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
//		
//	    GPIO_Initure.Pin= GPIO_PIN_15 ; 				//PB5
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
//    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
//    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
//    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
}

//#define Add_PIN  GPIO_PIN_15
//#define Dec_PIN  GPIO_PIN_12
//#define Alarm_PIN  GPIO_PIN_14
//#define Time_PIN  GPIO_PIN_11
//#define Watch_PIN  GPIO_PIN_15  //���
//#define Key_PIN  GPIO_PIN_15

void KEY_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
			//�ȸĳ�������м��
		GPIO_Initure.Pin=Add_PIN | Dec_PIN | Alarm_PIN| Time_PIN |Watch_PIN | Key_PIN; 
		GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
		GPIO_Initure.Pull=GPIO_PULLDOWN;          //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��WKUP���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!



u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //�����ɿ���־
    if(mode==1)key_up=1;    //֧������
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
    return 0;   //�ް�������
}


// Add_PRES  //Add_Key  
// Dec_PRES  //Dec_Key  
// Alarm_PRES//Alarm_Key
// Time_PRES //Time_Key 
// Watch_PRES//Watch_Key
// KEY_PRES  //KEY      