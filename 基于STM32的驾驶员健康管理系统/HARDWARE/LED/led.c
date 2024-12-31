#include "sys.h"
#include "led.h"
#include "delay.h"

void LED_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;//声明结构体——GPIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//时钟使能GPIOB
	 //RCC APB2PeriphClockcmd(RCC APB2Periph GPIOB | RCC APB2Periph AFIO,ENABLE); 
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//定义引脚pin_8 pin_9 pin_10 pin_11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//频率50Mhz
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化PA1-PA4,4个灯熄灭
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//GPIO_Pin_3|GPIO_Pin_4
}

void LED_shanshuo(void)
{
	 GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	 delay_ms(1000);   
	 GPIO_SetBits(GPIOB,GPIO_Pin_12);
	 delay_ms(1000);
}
