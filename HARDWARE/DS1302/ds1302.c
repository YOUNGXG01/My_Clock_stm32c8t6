#include "ds1302.h"


 DS_IO ds_io;
 uchar label2[16];//ʱ��������

//CLK �� RST ���ų�ʼ��
void DS1302_init(void)
{
	  GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();           	
	                                            
    GPIO_Initure.Pin=DS_CLK | DS_RST; 				
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	
    GPIO_Initure.Pull=GPIO_PULLUP;          	 
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;   
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}


//�����������ŵ�˫��IO
void DS1302_Data(DS_IO type_io)
{
		 GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_GPIOA_CLK_ENABLE();
		//����ģʽ
		if(type_io == DATA_IN)
		{
		GPIO_Initure.Pin=DS_DATA; 
		GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
		GPIO_Initure.Pull=GPIO_PULLUP;          //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
		DS_CLK_0;
		}
		else if(type_io == DATA_OUT)
		{
		GPIO_Initure.Pin=DS_DATA; 				
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	
    GPIO_Initure.Pull=GPIO_PULLUP;          	 
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);			
		}
}

/*****************************************
  * ��������void DS1302_Write(uchar add,uchar dat)
  * ����  ��DS1302дָ�������
 *  ����  ��add:���͵�ַ��dat����������
  * ���  ����
  * ����  ��
  *************************************/


  void DS1302_Write(uchar add,uchar dat)
  {
  uchar i,temp1,temp2;
  temp1=add;
  temp2=dat;
	ds_io = DATA_OUT;
	DS1302_Data(ds_io);
  RES_Set_1;//RET=1;    
 //���͵�ַ     
 for(i=0;i<8;i++)
  {
		 if(temp1&0x01)
			{DS_DATA_1;  }     //IO=1;      
		 else
		 { DS_DATA_0;}   //�IIO=0;     
		 temp1=temp1>>1;
		 DS_CLK_1;            //CLK=1;
		 delay_ms(2);    
		 DS_CLK_0;    //CLK=0;
		 delay_ms(2);  
   }
 
 //��������
 for(i=0;i<8;i++)
     {
         /*IO=(temp2>>i)&0x01;��һ������������ε�����  */
         if(temp2&0x01)
          {DS_DATA_1;  }     //IO=1;  
         else 
          { DS_DATA_0;}    //�IIO=0;
             temp2=temp2>>1;
             DS_CLK_1;            //CLK=1;
             delay_ms(2);    
             DS_CLK_0;    //CLK=0;
					delay_ms(2);  
     } 
	RES_Set_0;// RES=0;
  }
	
	
	/*****************************************
  * ��������uchar DS1302_Read(uchar add)
  * ����  ��DS1302������
  *  ����  ��add:�������ڵ�ַ
  * ���  ��
  * ����  ��
  *************************************/
 uchar DS1302_Read(uchar add)
 {
   uchar i,suf,temp,mm,nn,value;
    temp=add;
	  ds_io = DATA_OUT;
		DS1302_Data(ds_io);//����IOΪ���,�ָ�����״̬  
    RES_Set_1;//RET=1;  
     //д��ַ
   for(i=0;i<8;i++)
       {
        if(temp&0x01)
        {DS_DATA_1;  }     //IO=1; 
       else
          { DS_DATA_0;}    //IO=0;
       temp=temp>>1;
         DS_CLK_1;            //CLK=1;
         delay_ms(2);     
         DS_CLK_0;    //CLK=0;  
				 delay_ms(2);  
       }
  //������
		ds_io = DATA_IN;
			 
		DS1302_Data(ds_io);//����IOΪ����        
   for(i=0;i<8;i++)
     { 
      suf=suf>>1;//�����ݱ���
      if(IO_Read)    //IO=1
      {
          suf=suf|0x80; 
      }
      else     //IO=0
      {
      suf=suf&0x7f;  
      }
      
         DS_CLK_1;            //CLK=1;
         delay_ms(2);    
         DS_CLK_0;    //CLK=0;  
				delay_ms(2);  
     }
     RES_Set_0;    // RET=0;
		 
     ds_io = DATA_OUT;
		 DS1302_Data(ds_io);//����IOΪ���,�ָ�����״̬        
		 //���ݴ���ת��ʮ����    
		 mm=suf/16;
		 nn=suf%16;
		 value=mm*10+nn;
		 return value;        
 }
 
  /*****************************************
  * ��������void DS1302_SetTime(uchar *ad)
  * ����  ��DS1302 д������ʱ��
  *  ����  ��add:�������ڵ�ַ
  * ���  ��
  * ����  ��
  *************************************/
 void DS1302_SetTime(uchar *ad)    
   {     
			 DS1302_Write(0x8e,0x00);   //WP=0 ��������д��DS1302
 /**********���¶�ʱ����ĳ�ʼ��*************/
       DS1302_Write(ds1302_sec_addr,ad[5]);  //��
       DS1302_Write(ds1302_min_addr,ad[4]);   //��
       DS1302_Write(ds1302_hour_addr,ad[3]);   //ʱ
       /**********���¶������յĳ�ʼ��*************/
       DS1302_Write(ds1302_day_addr,ad[2]);
       DS1302_Write(ds1302_month_addr,ad[1]);
       DS1302_Write(ds1302_year_addr,ad[0]);     
         
       DS1302_Write(0x8e,0x80);   //0x8e�����ֽڵ�ַ,bit7=WP WP=1 ��ֹ����д��DS1302
   }
	 
	 
	 /*****************************************
  * �������� void DS1302_OFF(void)
  * ����  ��DS1302ʱ���ֹ��ʱ
  *  ����  ��
  * ���  ��
  * ����  ��
  *************************************/    
   void DS1302_OFF(void)
   {
    uchar temp;
    temp=DS1302_Read(0x81);//��ȡ���ַʱ��
    DS1302_Write(0x8e,0x00);//WP=0 ��������д��DS1302
    temp=temp|(1<<7);
    DS1302_Write(0x80,temp);//WP=1 ��ֹ����д��DS1302
   
   }
 /*****************************************
  * �������� void DS1302_ON(void)
  * ����  ��DS1302ʱ�俪ʼ���У���ʱ
  *  ����  ��
  * ���  ��
  * ����  ��
  *************************************/    
 void DS1302_ON(void)
 {
	uchar temp;
	temp=DS1302_Read(0x81);
	DS1302_Write(0x8e,0x00);//WP=0 ��������д��DS1302
	temp=temp|(0<<7);
	DS1302_Write(0x80,temp);//WP=0 ��������д��DS1302
 
 }
	 
	  /*****************************************
  * ��������void DS1302_init(uchar *time)
  * ����  ��    DS1302��ʼ��
  * ����  ����
  * ���  ����
 * ����  ��
  *************************************/
 void DS1302_Init(uchar *time)
 {
     DS1302_init();//GPIO��ʼ������
		 ds_io = DATA_OUT;
		 DS1302_Data(ds_io);
     delay_ms(10);  
//		 RES_Set_0; //RET=0;
//		 DS_CLK_0;// CLK=0;
//     //�����Ƕ�DS1302������أ�������   ����ʱ��
//     DS1302_Write(0x8e,0x00);//WP=0 ��������д��DS1302
//     DS1302_Write(0x90,0xA7);//���(1��������+8K����)
//     DS1302_Write(0x8E,0X80);//�������� WP=1
//     if(DS1302_Read(0X81)&0X80)//��ѯDS302ʱ���Ƿ�����,���ʱ��ֹͣ�߶�������ʱ��+��ʼ��ʱ��
//     { 
//         DS1302_SetTime(time);//�������ó�ʼʱ��
//     }
     //��������
     
 }
 
 void DS1302_Readtime(void)
 {
     label2[0]=DS1302_Read( 0x8D)/10;      //��:ʮλ
     label2[1]=DS1302_Read( 0x8D)%10;      //��:��λ
////     label2[2]=\'/\';
     label2[3]=DS1302_Read( 0x89)/10;     //�� ��
     label2[4]=DS1302_Read( 0x89)%10;     //�� ��
////     label2[5]=\'/\';
     label2[6]=DS1302_Read( 0x87)/10;     //��
     label2[7]=DS1302_Read( 0x87)%10;     //��
	 
     label2[8]=DS1302_Read( 0x85)/10;     //ʱ
     label2[9]=DS1302_Read( 0x85)%10;     //ʱ
////     label2[10]=\':\';    
     label2[11]=DS1302_Read( 0x83)/10;     //����
     label2[12]=DS1302_Read( 0x83)%10;     //����
//     label2[13]=\':\';
     label2[14]=DS1302_Read( 0x81)/10;     //���� 
     label2[15]=DS1302_Read( 0x81)%10;     //���� 
 }


