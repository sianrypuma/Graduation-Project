#include "stm32f10x.h"
#include  "key.h"
#include "delay.h"
void KEY_Init(void)
 {
	GPIO_InitTypeDef  GPIO_InitStructure;//声明结构体——GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//时钟使能GPIOB
	 //RCC APB2PeriphClockcmd(RCC APB2Periph GPIOB | RCC APB2Periph AFIO,ENABLE); 
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//定义引脚pin_8 pin_9 pin_10 pin_11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;    //上拉输入模式   Out_PP;//推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//频率50Mhz
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化PA1-PA4,4个灯熄灭
 }
  u8 KEY_Scan(void) //键值反馈
 {
		static u8 key_up=1;  //按键松开状态
	 if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
	 {
		 delay_ms(100);   //按键去抖
		 key_up=0;        //说明有闭合
		 if(KEY1==0)
		 {
			 return 1;
		 }
		 else if(KEY2==0)
		 {
		 return 2;
		 }
		  //else if(KEY3==0&&KEY4==0) 同时按响应功能
		 else if(KEY3==0)
		 {
		 return 3;
		 }
		 else if(KEY4==0)
		 {
		 return 4;
		 }
	 }
	 else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
		 key_up=1;
	 return 0;
} 