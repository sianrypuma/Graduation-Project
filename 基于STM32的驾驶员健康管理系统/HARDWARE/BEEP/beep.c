#include "sys.h"
#include "beep.h"

void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//声明结构体——GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//时钟使能GPIOB
	 //RCC APB2PeriphClockcmd(RCC APB2Periph GPIOB | RCC APB2Periph AFIO,ENABLE); 
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//定义引脚pin_8 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//频率50Mhz
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化GPIOB
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//高电平触发所以置位低电平
}
