#include "sys.h"
#include "beep.h"

void BEEP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;//�����ṹ�塪��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʱ��ʹ��GPIOB
	 //RCC APB2PeriphClockcmd(RCC APB2Periph GPIOB | RCC APB2Periph AFIO,ENABLE); 
	 //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//��������pin_8 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//Ƶ��50Mhz
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��GPIOB
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);//�ߵ�ƽ����������λ�͵�ƽ
}
