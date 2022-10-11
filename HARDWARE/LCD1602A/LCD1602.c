
#include "LCD1602.h"

/*���ų�ʼ��*/

void IO_Init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_AFIO_CLK_ENABLE();
		__HAL_AFIO_REMAP_SWJ_NOJTAG();
    __HAL_RCC_GPIOB_CLK_ENABLE();           	//����GPIOBʱ��
		__HAL_RCC_GPIOA_CLK_ENABLE();           	//����GPIOAʱ��
	
    GPIO_Initure.Pin= D0 | D1 | D2 | D3 | LCD_RS | LCD_R_W | LCD_E;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
	
	  GPIO_Initure.Pin=D4 | D5 | D6 | D7 ;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
    GPIO_Initure.Pull=GPIO_PULLUP;          	//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//��״̬
unsigned char ReadStatusLCD(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	 LCD_RS_0;
	 LCD_R_W_1;
	 LCD_E_1;
	
	//�ȸĳ�������м��
	GPIO_Initure.Pin=D7; 
	GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	while (Read_Busy); //���æ�ź�
	
	//Ȼ��ĳ�������м��
	GPIO_Initure.Pin=D7; 
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//�������
	GPIO_Initure.Pull=GPIO_PULLUP;          	//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	return 1;
}

void WriteInput(unsigned char WDLCD)
{
		//���ݷ���
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

                    
//д����

void WriteDataLCD(unsigned char WDLCD)
{
	 ReadStatusLCD(); //���æ
	 WriteInput(WDLCD);
	 LCD_RS_1;
	 LCD_R_W_0 ;
	 LCD_E_0; //�������ٶ�̫�߿���������С����ʱ
	 LCD_E_0; //��ʱ
	 LCD_E_1;
}

//дָ��
void WriteCommandLCD(unsigned char WCLCD,int BuysC) //BuysCΪ0ʱ����æ���
{
		 if(BuysC) ReadStatusLCD(); //���æ
	   WriteInput(WCLCD);
		 LCD_RS_0;
		 LCD_R_W_0 ;
		 LCD_E_0; //�������ٶ�̫�߿���������С����ʱ
		 LCD_E_0; //��ʱ
		 LCD_E_1;
}

void LCDInit(void) //LCM��ʼ��
{
 IO_Init();
 WriteInput(0);
 WriteCommandLCD(0x38,0); //������ʾģʽ���ã������æ�ź�
 delay_ms(5); 
 WriteCommandLCD(0x38,0);
 delay_ms(5); 
 WriteCommandLCD(0x38,0);
 delay_ms(5); 

 WriteCommandLCD(0x38,1); //��ʾģʽ����,��ʼҪ��ÿ�μ��æ�ź�
 WriteCommandLCD(0x08,1); //�ر���ʾ
 WriteCommandLCD(0x01,1); //��ʾ����
 WriteCommandLCD(0x06,1); // ��ʾ����ƶ�����
 WriteCommandLCD(0x0C,1); // ��ʾ�����������
}


//��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	 Y &= 0x1;
	 X &= 0xF; //����X���ܴ���15��Y���ܴ���1
	 if (Y) X |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
	 X |= 0x80; // ���ָ����
	 WriteCommandLCD(X, 0); //���ﲻ���æ�źţ����͵�ַ��
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
		DisplayOneChar(0,0,label2[0] + '0');  //��
		delay_ms(10);
		DisplayOneChar(1,0,label2[1] + '0');
	
		delay_ms(10);
		DisplayOneChar(2,0,'-');  //��
	
		delay_ms(10);
		DisplayOneChar(3,0,label2[3] + '0');  //��
		delay_ms(10);
		DisplayOneChar(4,0,label2[4] + '0');
	
		delay_ms(10);
		DisplayOneChar(5,0,'-');  //��
	
		delay_ms(10);
		DisplayOneChar(6,0,label2[6] + '0');  //��
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
	WriteCommandLCD(0x01,1); //��ʾ����
}
