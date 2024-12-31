#include "exti.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "usart.h"


/*
1.����ṹ���ʼ��
2.ʹ�ܸ��ù���ʱ��
3.�������š�ģʽ������
4.�ⲿ�ж�ͨ�����ȼ�
*/

void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO�ṹ��
	EXTI_InitTypeDef  EXTI_InitStructure; //����exti�ṹ��
	NVIC_InitTypeDef  NVIC_InitStructure;//����nvic�ṹ��
	
	//KEY_Init();  //�����˿ڳ�ʼ��
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //ʹ�ܸ��ù���ʱ��
	 //init GPIOA.0	  ��������
		/*GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);*/

	 //GPIOA.0
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	//GPIOE.2 �ж����Լ��жϳ�ʼ������   ���ش���
	EXTI_InitStructure.	EXTI_Line = EXTI_Line0;//KEY0
	EXTI_InitStructure.	EXTI_Mode = EXTI_Mode_Interrupt;//ģʽ���жϣ�
	EXTI_InitStructure.	EXTI_Trigger = EXTI_Trigger_Rising;//�½��ش���
	EXTI_InitStructure.	EXTI_LineCmd = ENABLE;  //ʹ��
	EXTI_Init(&EXTI_InitStructure);  //����exti_initstruct��ָ���Ĳ�����ʼ������exti�Ĵ���
	
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =	0x02;//��ռ���ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
		NVIC_Init(&NVIC_InitStructure);//����nvic_initstruct��ָ���Ĳ�������ʼ������nvic�Ĵ���
	
}

void EXTI0_IRQHandler()
 {
	 delay_ms(10);
	 if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		int i;
		delay_ms(10);
		if(KEY1==0)
		{
		//unsigned char temp_table[] ={"1234567891234567"};for(i=0;i<16;i++)    lcd_char_write(i,0,temp_table[i]);
			GPIO_Write(GPIOC,0x00);
			delay_ms(100);
		}
	}EXTI_ClearITPendingBit(EXTI_Line0);
 }




/*void EXTI2_IRQHandler(void)
{
	int i;
	delay_ms(10);
	if(KEY==0)
	{
		//unsigned char temp_table[] ={"1234567891234567"};for(i=0;i<16;i++)    lcd_char_write(i,0,temp_table[i]);
			GPIO_ResetBits(GPIOC,GPIO_Pin_0);
			delay_ms(100);
			GPIO_ResetBits(GPIOC,GPIO_Pin_1);
			delay_ms(100);
	
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


//�ⲿ�жϳ�ʼ������
//��ʼ��PA0,PA13,PA15Ϊ�ж�����.
void EXTIX_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;


  	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

  //init GPIOA.13,GPIOA.15  ��������
  	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
 //init GPIOA.0	  ��������
  	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  //GPIOA.13
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource13);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

 //GPIOA.15
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

  //GPIOA.0
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�4λ,��16��
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//��ռ���ȼ�0λ,�����ȼ�4λ
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
  
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�4λ,��16��
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//��ռ���ȼ�0λ,�����ȼ�4λ
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	
}

 
void EXTI0_IRQHandler(void)
{
    delay_ms(10);    //����
  	if(EXTI_GetITStatus(EXTI_Line0) != RESET)	  //���ָ����EXTI0��·�������������
	{	  
		LED0=!LED0;
		LED1=!LED1;	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //���EXTI0��·����λ
}
 
void EXTI15_10_IRQHandler(void)
{
  	delay_ms(10);    //����			 
 
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
	 	LED0=!LED0;
	}
 
     else if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		LED1=!LED1;
	}
	 EXTI_ClearITPendingBit(EXTI_Line13);  //���EXTI13��·����λ
	 EXTI_ClearITPendingBit(EXTI_Line15);  //���EXTI15��·����λ
}*/
