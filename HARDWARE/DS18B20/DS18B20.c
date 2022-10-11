#include "DS18B20.h"

unsigned char temp;
unsigned int sdata_t;

void DS18B20_Init_Out(void)
{
		 GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOB时钟

	//注意这里的输出一定要设置成为开漏模式 不然数据不稳定
	GPIO_Initure.Pin=DQ; 				//PA9
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  	//推挽输出 设置成开漏输出很稳定
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}
 
void DS18B20_Init_In(void)
{
	 GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOB时钟

	GPIO_Initure.Pin=DQ; 				//PB5
	GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
	GPIO_Initure.Pull=GPIO_NOPULL;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

 void DS18B20_Rst(void)	   
{                 
		DS18B20_Init_Out(); 	//SET PG11 OUTPUT
    DQ_0; 	//拉低DQ
    delay_us(750);    	//拉低750us
    DQ_1; 	//DQ=1 
		delay_us(15);     	//15US
}

unsigned char DS18B20_Read_Bit(void)
{
		unsigned char data;
		DS18B20_Init_Out();
		DQ_0;
		delay_us(2);
		DQ_1;
		DS18B20_Init_In();
		delay_us(12);
		if(Read_DQ) data=1;
		else data=0;
		delay_us(50);
		return data;
}


 //读一个字节
unsigned char DS18B20_Read_Byte(void)  			//主机数据线先从高拉至低电平1us以上，再使数据线升为高电平，从而产生读信号
{
		unsigned char i,j,dat;
    dat=0;
		for (i=1;i<=8;i++) 
		{
		j=DS18B20_Read_Bit();
		dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}


void DS18B20_Write_Byte(u8 dat)     
 {
		u8 j;
    u8 testb;	 
		DS18B20_Init_Out();	//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DQ_0;	// Write 1
            delay_us(2);                            
            DQ_1;
            delay_us(60);             
        }
        else 
        {
            DQ_0;	// Write 0
            delay_us(60);             
						DQ_1;
            delay_us(2);                          
        }
    }
}



//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_Init_In();	//SET PG11 INPUT	 
    while (Read_DQ&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!Read_DQ&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
//开始温度转换
void DS18B20_Start(void) 
{   						               
    DS18B20_Rst();	   
		DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0x44);	// convert
} 


short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
		short tem;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);	// skip rom
    DS18B20_Write_Byte(0xbe);	// convert	    
    TL=DS18B20_Read_Byte(); 	// LSB   
    TH=DS18B20_Read_Byte(); 	// MSB  
	    	  
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;					//温度为负  
    }else temp=1;				//温度为正	  	  
    tem=TH; 					//获得高八位
    tem<<=8;    
    tem+=TL;					//获得底八位
    tem=(float)tem*0.625;		//转换     
		if(temp)return tem; 		//返回温度值
		else return -tem;    
}


