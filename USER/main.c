#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "LCD1602.h"
#include "ds1302.h"
#include "stmflash.h"
#include "DS18B20.h"
#include "time.h"
#include "timer.h"

/************************************************

 //2022-10-10���
 // ����һ����ʱ����Ϊ���
 // KEYΪ�˳���
 
 
************************************************/

/*������������ʹ��HAL�⺯��ʵ�ֿ���IO�����*/

#define _key	0x06 //�ı����ֵ�Ϳ������³�ʼ��ʱ��
 u8 Settime[6]={0x22,0x10,0x10,0x11,0x30,0x10};  // ��ʼʱ������: �� �� ��  ʱ �� ��
 
// u8 TEXT_Buffer[]={"STM32F103 FLASH TEST"};
 u8 TEXT_Buffer[] = {_key};//�൱��һ���ܳ�
#define SIZE sizeof(TEXT_Buffer)		//���鳤��
	
	//���ַ0x08010000 ��Ϊc8t6��Ƭ��ֻ��64KB��flash�ռ�  �������Ҫ һ��Ҫ����������Ƭ���Ĵ���
//��Ƭ���ڲ�������㹫ʽ Code + RO-data + RW-data + ZI-data ��λ���ֽ�
#define FLASH_SAVE_ADDR  0X08007000	//flash д��ĵ�ַ


//����ȫ�ֱ����ı�־λ
int watch_flag = 0;
int set_flag = 0;
int time_break = 0;

//��ֹ������ܼ�ͬʱ���� ����ʹ��
int disturb_flag = 0;
//ʱ�ӱ�־λ
int hh_1,hh_0,mm_1,mm_0,ss_1,ss_0;
int turn1 = 0,turn2 = 0,turn3 = 0,turn4 = 0,turn5 = 0,turn6 = 0;
int chose = 0;

#define Delay_LCD 	delay_ms(10)   //����LCD1602��Ļ��Ҫ����ʱ ��Ȼ��ʾ��λ�û����


//�¶ȵ�ȫ�ֱ���
short tep = 0;
//�¶ȱ�־λ
int temp_flag = 0;

//���������Ķ��� ʹ�ó������Ӹ��ӿ��
__inline void Test_DS1302_Init(void);
__inline void Switch_watch(void);
__inline void show_stopwatch(void);
__inline void Swtich_Break(void);
__inline void Switch_Add(void);
__inline void sub(void);//�㲻��Ϊɶ��������������ʹ�ã�
__inline void show_temp(void);
__inline void Swtich_Time(void);
__inline void get_temp(void);
__inline void set_Time(void);
__inline void show_time(int h1,int h0,int m1,int m0,int s1,int s0);
__inline void Switch_Sub(void);
__inline void Switch_Set(void);

//u8 Ret_time[6] = {0x22,0x10,0x09,(hh_1*10 + hh_0),(mm_1*10 + mm_0),(ss_1*10 + ss_0)};
int main(void)
{
		u8 key;
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
		delay_init(72);               		//��ʼ����ʱ����
		LED_Init();							//��ʼ��LED	
		delay_ms(10);
		LCDInit();
		delay_ms(10);
		KEY_Init();
		Test_DS1302_Init();
		delay_ms(10);
		TIM3_Init(100-1,7200-1);       	//��ʱ��3��ʼ������ʱ��ʱ��Ϊ72M����Ƶϵ��Ϊ7200-1�� 10ms�Ķ�ʱ��
		__HAL_RCC_TIM3_CLK_DISABLE();
		stop_watch = 0;
	while(1)
	{
		if((!watch_flag && !set_flag) || time_break == 1 )
		{
				Show_T();	
				get_temp();
				time_break = 0;
		}
		

		key=KEY_Scan(0);            //����ɨ��
			switch(key)
		{				 
			case Add_PRES:
				if(!disturb_flag || set_flag)
				Switch_Add();
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				break;
			case Dec_PRES:
				if(!disturb_flag || set_flag)
				Switch_Sub();
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				break;
			case Time_PRES:
				if(!disturb_flag || set_flag)
				{
					//ʱ���� �ߵ�λ  �����պ����ټ�
					set_flag++;				
				}					
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				break;
			case Alarm_PRES:  //���Ӽ��ĳɵ�ȷ�ϰ�ť
				if(!disturb_flag || set_flag)
				Switch_Set();
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				break;
			case Watch_PRES:
				if(!disturb_flag || watch_flag)
				Switch_watch();	
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);				
				break;
			case KEY_PRES:
					Swtich_Break();
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				break;			
			default:
				delay_ms(10);	
		}
		
		

		if(set_flag)
		{
			Swtich_Time();
			delay_ms(500);
		}
		if(watch_flag)
		{
			show_stopwatch();
		}

	}
}




//-----------һЩ�����Ķ���

//swtich ����ѡ����

__inline void Switch_Set(void)
{
					Delay_LCD;
					DS1302_Write(0x8e,0x00);   //WP=0 ��������д��DS1302
 /**********���¶�ʱ����ĳ�ʼ��*************/
				 DS1302_Write(ds1302_sec_addr,(u8)(ss_1*16 + ss_0));  //��
				 DS1302_Write(ds1302_min_addr,(u8)(mm_1*16 + mm_0));   //��
				 DS1302_Write(ds1302_hour_addr,(u8)(hh_1*16 + hh_0));   //ʱ
				 HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}


__inline void Switch_Sub(void)
{
					if(set_flag)
				{	
					switch(chose)
					{
						case 1:
							ss_0--;
						break;
						case 2:
							ss_1--;
						break;
						case 3:
							mm_0--;
						break;
						case 4:
							mm_1--;
						break;
						case 5:
							hh_0--;
						break;
						case 6:
							hh_1--;
						break;
						default:
						break;
					};
					if(ss_0 == 0) ss_0 = 9;
					if(ss_1 == 0) ss_1 = 9;
					if(mm_0 == 0) mm_0 = 9;
					if(mm_1 == 0) mm_1 = 9;
					if(hh_0 == 0) hh_0 = 9;
					if(hh_1 == 0) hh_1 = 9;
				}

}


__inline void Switch_watch(void)
{
				disturb_flag = 1;
				Delay_LCD;
				watch_flag++;
				if(watch_flag == 4)
				watch_flag = 1;
			if(watch_flag == 1)
			{
				__HAL_RCC_TIM3_CLK_ENABLE();
				delay_ms(10);
				LCD_Clear();
				Delay_LCD;
				DisplayOneChar(11,1,'S');
				Delay_LCD;
				DisplayOneChar(12,1,'t');
				Delay_LCD;
				DisplayOneChar(13,1,'a');
				Delay_LCD;
				DisplayOneChar(14,1,'r');
				Delay_LCD;
				DisplayOneChar(15,1,'t');
			}
			else if(watch_flag == 2)
			{
				__HAL_RCC_TIM3_CLK_DISABLE();
				
				Delay_LCD;
				LCD_Clear();
				Delay_LCD;
				DisplayOneChar(11,1,'P');
				Delay_LCD;
				DisplayOneChar(12,1,'a');
				Delay_LCD;
				DisplayOneChar(13,1,'u');
				Delay_LCD;
				DisplayOneChar(14,1,'s');
				Delay_LCD;
				DisplayOneChar(15,1,'e');
				
			}
			else if(watch_flag == 3)
			{
				
				Delay_LCD;
				LCD_Clear();
				Delay_LCD;
				DisplayOneChar(11,1,'E');
				Delay_LCD;
				DisplayOneChar(12,1,'n');
				Delay_LCD;
				DisplayOneChar(13,1,'d');
				
				stop_watch = 0;
			}
				else if(watch_flag == 4)
				{
					Delay_LCD;
					LCD_Clear();
				}
				
				Delay_LCD;
				DisplayOneChar(4,1,':');
				Delay_LCD;
				DisplayOneChar(7,1,':');
				
				
				Delay_LCD;
				DisplayOneChar(4,0,'S');
				Delay_LCD;        
				DisplayOneChar(5,0,'t');
				Delay_LCD;        
				DisplayOneChar(6,0,'o');
				Delay_LCD;        
				DisplayOneChar(7,0,'p');
				Delay_LCD;        
				DisplayOneChar(8,0,'w');
				Delay_LCD;        
				DisplayOneChar(9,0,'a');
				Delay_LCD;        
				DisplayOneChar(10,0,'t');
				Delay_LCD;        
				DisplayOneChar(11,0,'c');
				Delay_LCD;        
				DisplayOneChar(12,0,'h');
}

__inline void Swtich_Break(void)

{
		disturb_flag = 0; //�����ʾ�����������԰���
		time_break = 1;
		if(watch_flag)
		{
			watch_flag = 0;
			stop_watch = 0;//��������
			disturb_flag = 0;
			__HAL_RCC_TIM3_CLK_DISABLE();
		}
			else if(set_flag)
			set_flag = 0;
			Delay_LCD;
			LCD_Clear();
}

__inline void Swtich_Time(void)
{
						disturb_flag = 1;
						set_Time();
					if(set_flag == 1)
					{
					set_flag = 2;
					ss_0 = label2[15];
					ss_1 = label2[14];
					mm_0 = label2[12];
					mm_1 = label2[11];
					hh_0 = label2[9];
					hh_1 = label2[8];
					show_time(hh_1,hh_0,mm_1,mm_0,ss_1,ss_0);
					}
					else if(set_flag > 1)
					{
						switch(set_flag)
						{
							case 2:
								chose = 1;
								if(0 == turn1)
								{
									turn1 = 1;
									Delay_LCD;
									DisplayOneChar(9,1,'X');
								}
								else
								{
									turn1 = 0;
									Delay_LCD;
									DisplayOneChar(9,1,ss_0 + '0');
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(2,1,hh_1 + '0');
								};
								break;
								case 3:
										chose = 2;
								if(0 == turn2)
								{
									turn2 = 1;
									Delay_LCD;
									DisplayOneChar(8,1,'X');
								}
								else
								{
									turn2 = 0;
									Delay_LCD;
									DisplayOneChar(8,1,ss_1 + '0');
									Delay_LCD;
									DisplayOneChar(9,1,ss_0 + '0');

								};
							break;
							case 4:
										chose = 3;
								if(0 == turn3)
								{
									turn3 = 1;
									Delay_LCD;
									DisplayOneChar(6,1,'X');
								}
								else
								{
									turn3 = 0;
									Delay_LCD;
									DisplayOneChar(8,1,ss_1 + '0');
									Delay_LCD;
									DisplayOneChar(6,1,mm_0 + '0');
								};
							break;
							case 5:
									chose = 4;
								if(0 == turn4)
								{
									turn4 = 1;
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(5,1,'X');
								}
								else
								{
									turn4 = 0;
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(6,1,mm_0 + '0');
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(5,1,mm_1 + '0');
								};
							break;
							case 6:
										chose = 5;
								if(0 == turn5)
								{
									turn5 = 1;
									Delay_LCD;
									DisplayOneChar(3,1,'X');
								}
								else
								{
									turn5 = 0;
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(5,1,mm_1 + '0');
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(3,1,hh_0 + '0');
								};
							break;	
							case 7:
										chose = 6;
								if(0 == turn6)
								{
									turn6 = 1;
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(2,1,'X');
								}
								else
								{
									turn6 = 0;
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(3,1,hh_0 + '0');
									Delay_LCD;
									Delay_LCD;
									DisplayOneChar(2,1,hh_1 + '0');
								};
							break;	
							default:
								set_flag = 2;
							break;
						}
					}
}
//���������ʾ
__inline void show_stopwatch(void)
{
	//��ʾð��
	if(stop_watch < 10)
	{
		delay_ms(10);
		DisplayOneChar(4,1,':');
		delay_ms(10);
		DisplayOneChar(7,1,':');
	}
	{
		delay_ms(10);
		DisplayOneChar(9,1,stop_watch % 10 + '0');
		delay_ms(10);
		DisplayOneChar(8,1,(stop_watch/10) % 10 + '0');
		
		delay_ms(10);
		DisplayOneChar(6,1,((stop_watch/100)%60)%10 + '0');
		delay_ms(10);
		DisplayOneChar(5,1,((stop_watch/100)%60)/10 + '0');
		
		delay_ms(10);
		DisplayOneChar(3,1,((stop_watch /100)/60)%10 + '0');
		delay_ms(10);
		DisplayOneChar(2,1,((stop_watch /100)/60)/10 + '0');
	}
	
}

__inline void Test_DS1302_Init(void)
{
		u8 datatemp[SIZE];
		u16 temp;
		STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);
	//�����ʼ�����Ͳ��ٽ��г�ʼ����
	if(datatemp[0] != _key)
	{
				STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);
				DS1302_Init(Settime);                                                                //ʱ���ʼ��
				DS1302_SetTime(Settime);
	}
	else
	{
		DS1302_Init(Settime); 
	}
}


__inline void show_temp(void)
{
			Delay_LCD;
			DisplayOneChar(10,0,'T');
			Delay_LCD;
			DisplayOneChar(11,0,'e');
			Delay_LCD;
			DisplayOneChar(12,0,'m');
			Delay_LCD;
			DisplayOneChar(13,0,'p');
			Delay_LCD;
			DisplayOneChar(14,0,':');
}

__inline void Switch_Add(void)
{
						if(set_flag)
				{	
					switch(chose)
					{
						case 1:
							ss_0++;
						break;
						case 2:
							ss_1++;
						break;
						case 3:
							mm_0++;
						break;
						case 4:
							mm_1++;
						break;
						case 5:
							hh_0++;
						break;
						case 6:
							hh_1++;
						break;
						default:
						break;
					};
					if(ss_0 == 10) ss_0 = 0;
					if(ss_1 == 10) ss_1 = 0;
					if(mm_0 == 10) mm_0 = 0;
					if(mm_1 == 10) mm_1 = 0;
					if(hh_0 == 10) hh_0 = 0;
					if(hh_1 == 10) hh_1 = 0;
				}
}


__inline void get_temp(void)
{
		tep = DS18B20_Get_Temp();
		
		Delay_LCD;
		DisplayOneChar(12,1,tep/100 +  '0');
		
		Delay_LCD;
		DisplayOneChar(13,1,((tep/10)%10) + '0');
		
		Delay_LCD;
		DisplayOneChar(14,1,'.');
		
		Delay_LCD;
		DisplayOneChar(15,1,(tep%10) + '0');
		
	if(time_break == 1 || temp_flag == 0)  //ֻ���ڴη��ص�ʱ����ʾ
	{
			temp_flag = 1;
			Delay_LCD;
			DisplayOneChar(11,0,'T');
			Delay_LCD;
			DisplayOneChar(12,0,'e');
			Delay_LCD;
			DisplayOneChar(13,0,'m');
			Delay_LCD;
			DisplayOneChar(14,0,'p');
			Delay_LCD;
			DisplayOneChar(15,0,':');
	}
}

__inline void set_Time(void)
{
				Delay_LCD;
			DisplayOneChar(3,0,'S');
			Delay_LCD;
			DisplayOneChar(4,0,'e');
			Delay_LCD;
			DisplayOneChar(5,0,'t');
			Delay_LCD;
			DisplayOneChar(8,0,'T');
			Delay_LCD;
			DisplayOneChar(9,0,'i');
			Delay_LCD;
			DisplayOneChar(10,0,'m');
			Delay_LCD;
			DisplayOneChar(11,0,'e');
}
__inline void show_time(int h1,int h0,int m1,int m0,int s1,int s0)
{
					LCD_Clear();
					Delay_LCD;
					DisplayOneChar(9,1,s0 + '0');
					Delay_LCD;
					DisplayOneChar(8,1,s1 + '0');
					Delay_LCD;
					
					DisplayOneChar(7,1,':');
					
					Delay_LCD;
					DisplayOneChar(6,1,m0 + '0');
					Delay_LCD;
					DisplayOneChar(5,1,m1+ '0');
					Delay_LCD;
					
					DisplayOneChar(4,1,':');
					Delay_LCD;
					DisplayOneChar(3,1,h0 + '0');
					Delay_LCD;
					DisplayOneChar(2,1,h1 + '0');

}


