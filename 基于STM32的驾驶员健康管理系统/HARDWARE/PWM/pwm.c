#include "stm32f10x.h"
#include "pwm.h"

void PWM_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;//输出比较的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//时钟使能GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //复用时开启
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE); //重映射
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); //解除PA15调试功能
	

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//定义引脚pin_0|Pin_15
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出模式，改变引脚控制权给片上外设
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//频率50Mhz
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM2);
	
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision =TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 100-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到20为2ms  //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  //psc
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	
	TIM_OCStructInit(&TIM_OCInitStructure);//用StructInit赋初始值函数，可避免语法错误
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//输出比较的模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出比较的使能
	TIM_OCInitStructure.TIM_Pulse = 0;//输出比较配置CCR条件码寄存器  //占空比50%  占空比10%   占空比90%  
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);//查看引脚定义表知道,非重映射默认只有PA0输出
	/*公式：PWM频率：Freq = CK_PSC/(PSC+1)/(ARR+1)
					PWM占空比：Doty = CCR/(ARR+1)
					PWM分辨率：Reso = 1/(ARR+1)   */

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
							 
}

void PWM_SetCompare1(uint16_t Compare)//单独更改通道1的值对应TIM_OC1Init ，从而改变CCR实现占空比变化
{
	TIM_SetCompare1(TIM2, Compare);
}
