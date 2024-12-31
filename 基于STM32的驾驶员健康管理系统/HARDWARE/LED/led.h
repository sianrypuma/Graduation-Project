
#ifndef __LED_H
#define __LED_H	 
//LED端口定义
//#define LED0 PAout(8)// PA8
#define LED1 PBout(12)// PB12	
#define LED2 PBout(13)// PB13	
#define LED3 PBout(14)// PB14 

void LED_Init(void);//初始化
void LED_shanshuo(void);
//void display(void);
//void displaybuffer(void);
		 				    
#endif
