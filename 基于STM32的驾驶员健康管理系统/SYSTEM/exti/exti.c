#include "exti.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "usart.h"


/*
1.定义结构体初始化
2.使能复用功能时钟
3.配置引脚、模式、触发
4.外部中断通道优先级
*/

void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO结构体
	EXTI_InitTypeDef  EXTI_InitStructure; //定义exti结构体
	NVIC_InitTypeDef  NVIC_InitStructure;//定义nvic结构体
	
	//KEY_Init();  //按键端口初始化
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //使能复用功能时钟
	 //init GPIOA.0	  下拉输入
		/*GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
  	GPIO_Init(GPIOA, &GPIO_InitStructure);*/

	 //GPIOA.0
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	//GPIOE.2 中断线以及中断初始化配置   降沿触发
	EXTI_InitStructure.	EXTI_Line = EXTI_Line0;//KEY0
	EXTI_InitStructure.	EXTI_Mode = EXTI_Mode_Interrupt;//模式（中断）
	EXTI_InitStructure.	EXTI_Trigger = EXTI_Trigger_Rising;//下降沿触发
	EXTI_InitStructure.	EXTI_LineCmd = ENABLE;  //使能
	EXTI_Init(&EXTI_InitStructure);  //根据exti_initstruct中指定的参数初始化外设exti寄存器
	
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//使能按键KEY0所在的外部中断通道
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =	0x02;//抢占优先级2
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
		NVIC_Init(&NVIC_InitStructure);//根据nvic_initstruct中指定的参数化初始化外设nvic寄存器
	
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


//外部中断初始化程序
//初始化PA0,PA13,PA15为中断输入.
void EXTIX_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;


  	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

  //init GPIOA.13,GPIOA.15  上拉输入
  	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
 //init GPIOA.0	  下拉输入
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


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//先占优先级4位,共16级
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//先占优先级0位,从优先级4位
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
  
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//先占优先级4位,共16级
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//先占优先级0位,从优先级4位
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	
}

 
void EXTI0_IRQHandler(void)
{
    delay_ms(10);    //消抖
  	if(EXTI_GetITStatus(EXTI_Line0) != RESET)	  //检查指定的EXTI0线路触发请求发生与否
	{	  
		LED0=!LED0;
		LED1=!LED1;	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);  //清除EXTI0线路挂起位
}
 
void EXTI15_10_IRQHandler(void)
{
  	delay_ms(10);    //消抖			 
 
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
	 	LED0=!LED0;
	}
 
     else if (EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		LED1=!LED1;
	}
	 EXTI_ClearITPendingBit(EXTI_Line13);  //清除EXTI13线路挂起位
	 EXTI_ClearITPendingBit(EXTI_Line15);  //清除EXTI15线路挂起位
}*/
