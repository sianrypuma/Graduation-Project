#include "stm32f10x.h"
#ifndef __KEY_H
#define __KEY_H
//#define KEY  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define KEY1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define KEY2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define KEY3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define KEY4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
void KEY_Init(void);
u8 KEY_Scan(void);
#endif
