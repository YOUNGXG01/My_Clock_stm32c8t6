
#include "LCD1602.h"

/*引脚初始化*/

void IO_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_SWJ_NOJTAG();
    __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOB时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟
	
    GPIO_Initure.Pin= D0 | D1 | D2 | D3 | LCD_RS | LCD_R_W | LCD_E;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	
	  GPIO_Initure.Pin=D4 | D5 | D6 | D7 ;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//读状态
unsigned char ReadStatusLCD(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	 LCD_RS_0;
	 LCD_R_W_1;
	 LCD_E_1;
	
	//先改成输入进行检测
	GPIO_Initure.Pin=D7; 
	GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	while (Read_Busy); //检测忙信号
	
	//然后改成输出进行检测
	GPIO_Initure.Pin=D7; 
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          	//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	return 1;
}

void WriteInput(unsigned char WDLCD)
{
		//数据发送
	{
	   if( WDLCD & 0x1 ) 
			D0_1;
	 else
			D0_0;
	 	 if( WDLCD & 0x2 ) 
			D1_1;
	 else
			D1_0;
	 	 if( WDLCD & 0x4 ) 
			D2_1;
	 else
			D2_0;
	 	 if( WDLCD & 0x8 ) 
			D3_1;
	 else
			D3_0;
	 	 if( WDLCD & 0x10 ) 
			D4_1;
	 else
			D4_0;
	 	 if( WDLCD & 0x20 ) 
			D5_1;
	 else
			D5_0;
	 	 if( WDLCD & 0x40 ) 
			D6_1;
	 else
			D6_0;
	 	 if( WDLCD & 0x80 ) 
			D7_1;
	 else
			D7_0;
 }

}

                    
//写数据

void WriteDataLCD(unsigned char WDLCD)
{
	 ReadStatusLCD(); //检测忙
	 WriteInput(WDLCD);
	 LCD_RS_1;
	 LCD_R_W_0 ;
	 LCD_E_0; //若晶振速度太高可以在这后加小的延时
	 LCD_E_0; //延时
	 LCD_E_1;
}

//写指令
void WriteCommandLCD(unsigned char WCLCD,int BuysC) //BuysC为0时忽略忙检测
{
		 if(BuysC) ReadStatusLCD(); //检测忙
	   WriteInput(WCLCD);
		 LCD_RS_0;
		 LCD_R_W_0 ;
		 LCD_E_0; //若晶振速度太高可以在这后加小的延时
		 LCD_E_0; //延时
		 LCD_E_1;
}

void LCDInit(void) //LCM初始化
{
 IO_Init();
 WriteInput(0);
 WriteCommandLCD(0x38,0); //三次显示模式设置，不检测忙信号
 delay_ms(5); 
 WriteCommandLCD(0x38,0);
 delay_ms(5); 
 WriteCommandLCD(0x38,0);
 delay_ms(5); 

 WriteCommandLCD(0x38,1); //显示模式设置,开始要求每次检测忙信号
 WriteCommandLCD(0x08,1); //关闭显示
 WriteCommandLCD(0x01,1); //显示清屏
 WriteCommandLCD(0x06,1); // 显示光标移动设置
 WriteCommandLCD(0x0C,1); // 显示开及光标设置
}


//按指定位置显示一个字符
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	 Y &= 0x1;
	 X &= 0xF; //限制X不能大于15，Y不能大于1
	 if (Y) X |= 0x40; //当要显示第二行时地址码+0x40;
	 X |= 0x80; // 算出指令码
	 WriteCommandLCD(X, 0); //这里不检测忙信号，发送地址码
	 WriteDataLCD(DData);
}


//void show_time(void)
//{
//	delay_ms(100); 
//	DisplayOneChar(2,0,'T');
//	DisplayOneChar(3,0,'i');
//	DisplayOneChar(4,0,'m');
//	DisplayOneChar(5,0,'e');
//	DisplayOneChar(6,0,':');
//}


void Show_T(void)
{
		DS1302_Readtime();
	
		delay_ms(10);
		DisplayOneChar(0,0,label2[0] + '0');  //年
		delay_ms(10);
		DisplayOneChar(1,0,label2[1] + '0');
	
		delay_ms(10);
		DisplayOneChar(2,0,'-');  //月
	
		delay_ms(10);
		DisplayOneChar(3,0,label2[3] + '0');  //月
		delay_ms(10);
		DisplayOneChar(4,0,label2[4] + '0');
	
		delay_ms(10);
		DisplayOneChar(5,0,'-');  //月
	
		delay_ms(10);
		DisplayOneChar(6,0,label2[6] + '0');  //日
		delay_ms(10);
		DisplayOneChar(7,0,label2[7] + '0');
	
		delay_ms(10);
		DisplayOneChar(9,1,label2[15] + '0');
		delay_ms(10);
		DisplayOneChar(8,1,label2[14] + '0');
		delay_ms(10);
		
		DisplayOneChar(7,1,':');
		
		delay_ms(10);
		DisplayOneChar(6,1,label2[12] + '0');
		delay_ms(10);
		DisplayOneChar(5,1,label2[11] + '0');
		delay_ms(10);
		
		DisplayOneChar(4,1,':');
		delay_ms(10);
		DisplayOneChar(3,1,label2[9] + '0');
		delay_ms(10);
		DisplayOneChar(2,1,label2[8] + '0');
		delay_ms(10);
}

void LCD_Clear(void)
{
	delay_ms(10);
	WriteCommandLCD(0x01,1); //显示清屏
}
