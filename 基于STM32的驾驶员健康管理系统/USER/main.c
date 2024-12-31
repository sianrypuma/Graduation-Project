/*********************************************************************************
 * 文件名  ：main.c
 * 描述    ：通过stm32的spi1读取max6675的温度值，并通过uart1发送出来
 *          
 * 实验平台：STM32开发板
 * 库版本  ：ST3.0.0
 * 硬件连接： ------------------------------------
 *           |PA6-SPI1-MISO：MAX6675-SO          |
 *           |PA7-SPI1-MOSI：MAX6675-SI          |
 *           |PA5-SPI1-SCK ：MAX6675-SCK         |
 *           |PA4-SPI1-NSS ：MAX6675-CS          |
 *            ------------------------------------
**********************************************************************************/
#include "stm32f10x.h"
#include "oled.h"
#include "stdio.h"
#include "delay.h"
#include "ds18b20.h"
#include "max30102.h"
#include "iicStart.h"
#include "max30102_math.h"
#include "led.h"

#define MAX_BRIGHTNESS 255

int32_t n_ir_buffer_length = 150; //data length
int32_t i,j,k;
uint32_t aun_ir_buffer[150]; //infrared LED sensor data
uint32_t aun_red_buffer[150];  //red LED sensor data
uint32_t un_min = 0x3fff, un_max = 0 ,un_prev_data,un_brightness;

int32_t n_spo2;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
unsigned char manle;
float f_temp;

int32_t hrTimeout = 0;
int32_t hrValidCnt = 0;
int32_t hrThrowOutSamp = 0;
int32_t hr_buf[16];

int32_t hrBuffFilled;
int32_t hrSum;
int32_t hrAvg;

int32_t spo2Timeout = 0;
int32_t spo2ValidCnt = 0;
int32_t spo2ThrowOutSamp = 0;
int32_t spo2_buf[16];
int32_t spo2Sum;
int32_t spo2Avg;
int32_t spo2BuffFilled;     
uint8_t uch_dummy;
int32_t spo2Avg_arg1[30];      		 	//注意注意  我的spo2Avg_arg1[15] 就是最后获取的血氧数据
int32_t hrAvg_arg1[30],temp;    			 //注意注意  我的hrAvg_arg1[15] 就是最后获取的心率数据

uint8_t ucbuf[17];
float temp_data=0.0;
float temp_data1=0.0;

void Gain_Hr_Spo(void);

void TEMP_Value_Conversion()
{
	temp_data=DS18B20_Get_Temp();
	temp_data1=temp_data/10.0;
	
	sprintf((char*)ucbuf,"%4.2f",temp_data1);
  OLED_ShowString(72,48,(uint8_t*)ucbuf,16,1);
  OLED_ShowChinese(0,48,0,16,1);
	OLED_ShowChinese(16,48,1,16,1);
	OLED_ShowChinese(32,48,2,16,1);
	OLED_ShowChinese(48,48,3,16,1);
	OLED_ShowChar(64,48,':',16,1);
	OLED_ShowChinese(112,48,4,16,1);
	  
}

/*
函数功能：获取心率和血氧
函数参数：无
函数返回值：无
函数描述：无
*/
void Gain_Hr_Spo(void)
{
	i = 0;
	un_min = 0x3FFFF;
	un_max = 0;
 //dumping the first 50 sets of samples in the memory and shift the last 100 sets of samples to the top
		for(i = 50; i < 150; i++)
		{
				aun_red_buffer[i - 50] = aun_red_buffer[i];
				aun_ir_buffer[i - 50] = aun_ir_buffer[i];

				//update the signal min and max
				if(un_min > aun_red_buffer[i])
						un_min = aun_red_buffer[i];
				if(un_max < aun_red_buffer[i])
						un_max = aun_red_buffer[i];
		}

		//take 50 sets of samples before calculating the heart rate.
		for(i = 100; i < 150; i++)
		{
				un_prev_data = aun_red_buffer[i - 1];
				maxim_max30102_read_fifo((aun_red_buffer + i), (aun_ir_buffer + i));

				//calculate the brightness of the LED
				if(aun_red_buffer[i] > un_prev_data)
				{
						f_temp = aun_red_buffer[i] - un_prev_data;
						f_temp /= (un_max - un_min);
						f_temp *= MAX_BRIGHTNESS;
						f_temp = un_brightness - f_temp;
						if(f_temp < 0)
								un_brightness = 0;
						else
								un_brightness = (int)f_temp;
				}
				else
				{
						f_temp = un_prev_data - aun_red_buffer[i];
						f_temp /= (un_max - un_min);
						f_temp *= MAX_BRIGHTNESS;
						un_brightness += (int)f_temp;
						if(un_brightness > MAX_BRIGHTNESS)
								un_brightness = MAX_BRIGHTNESS;
				}
		}
		maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

		if ((ch_hr_valid == 1) && (n_heart_rate < 190) && (n_heart_rate > 40))
		{
				hrTimeout = 0;

				// Throw out up to 1 out of every 5 valid samples if wacky
				if (hrValidCnt == 4)
				{
						hrThrowOutSamp = 1;
						hrValidCnt = 0;
						for (i = 12; i < 16; i++)
						{
								if (n_heart_rate < hr_buf[i] + 10)
								{
										hrThrowOutSamp = 0;
										hrValidCnt   = 4;
								}
						}
				}
				else
				{
						hrValidCnt = hrValidCnt + 1;
				}

				if (hrThrowOutSamp == 0)
				{

						// Shift New Sample into buffer
						for(i = 0; i < 15; i++)
						{
								hr_buf[i] = hr_buf[i + 1];
						}
						hr_buf[15] = n_heart_rate;

						// Update buffer fill value
						if (hrBuffFilled < 16)
						{
								hrBuffFilled = hrBuffFilled + 1;
						}

						// Take moving average
						hrSum = 0;
						if (hrBuffFilled < 2)
						{
								hrAvg = 0;
						}
						else if (hrBuffFilled < 4)
						{
								for(i = 14; i < 16; i++)
								{
										hrSum = hrSum + hr_buf[i];
								}
								hrAvg = hrSum >> 1;
						}
						else if (hrBuffFilled < 8)
						{
								for(i = 12; i < 16; i++)
								{
										hrSum = hrSum + hr_buf[i];
								}
								hrAvg = hrSum >> 2;
						}
						else if (hrBuffFilled < 16)
						{
								for(i = 8; i < 16; i++)
								{
										hrSum = hrSum + hr_buf[i];
								}
								hrAvg = hrSum >> 3;
						}
						else
						{
								for(i = 0; i < 16; i++)
								{
										hrSum = hrSum + hr_buf[i];
								}
								hrAvg = hrSum >> 4;
						}
				}
				hrThrowOutSamp = 0;
		}
		else
		{
				hrValidCnt = 0;
				if (hrTimeout == 4)
				{
						hrAvg = 0;
						hrBuffFilled = 0;
				}
				else
				{
						hrTimeout++;
				}
		}

		if ((ch_spo2_valid == 1) && (n_spo2 > 59))
		{
				spo2Timeout = 0;

				// Throw out up to 1 out of every 5 valid samples if wacky
				if (spo2ValidCnt == 4)
				{
						spo2ThrowOutSamp = 1;
						spo2ValidCnt = 0;
						for (i = 12; i < 16; i++)
						{
								if (n_spo2 > spo2_buf[i] - 10)
								{
										spo2ThrowOutSamp = 0;
										spo2ValidCnt   = 4;
								}
						}
				}
				else
				{
						spo2ValidCnt = spo2ValidCnt + 1;
				}

				if (spo2ThrowOutSamp == 0)
				{

						// Shift New Sample into buffer
						for(i = 0; i < 15; i++)
						{
								spo2_buf[i] = spo2_buf[i + 1];
						}
						spo2_buf[15] = n_spo2;

						// Update buffer fill value
						if (spo2BuffFilled < 16)
						{
								spo2BuffFilled = spo2BuffFilled + 1;
						}

						// Take moving average
						spo2Sum = 0;
						if (spo2BuffFilled < 2)
						{
								spo2Avg = 0;
						}
						else if (spo2BuffFilled < 4)
						{
								for(i = 14; i < 16; i++)
								{
										spo2Sum = spo2Sum + spo2_buf[i];
								}
								spo2Avg = spo2Sum >> 1;
						}
						else if (spo2BuffFilled < 8)
						{
								for(i = 12; i < 16; i++)
								{
										spo2Sum = spo2Sum + spo2_buf[i];
								}
								spo2Avg = spo2Sum >> 2;
						}
						else if (spo2BuffFilled < 16)
						{
								for(i = 8; i < 16; i++)
								{
										spo2Sum = spo2Sum + spo2_buf[i];
								}
								spo2Avg = spo2Sum >> 3;
						}
						else
						{
								for(i = 0; i < 16; i++)
								{
										spo2Sum = spo2Sum + spo2_buf[i];
								}
								spo2Avg = spo2Sum >> 4;
						}
				}
				spo2ThrowOutSamp = 0;
		}
		else
		{
				spo2ValidCnt = 0;
				if (spo2Timeout == 4)
				{
						spo2Avg = 0;
						spo2BuffFilled = 0;
				}
				else
				{
						spo2Timeout++;
				}
		}
		if(hrAvg != 0 && spo2Avg != 0)
		{
			hrAvg_arg1[j] = hrAvg;
			spo2Avg_arg1[j++] = spo2Avg;
			if(j == 30)                        //我觉得模块本身算法不是很精确 所以我自己弄了个本人而言还过得去的算法
			{																		//当j=30 代表已经获取了30次心率 血氧值了  然后我取30次里中间的值
				for(j = 0;j < 30;j++)        //这里两个循环是冒泡排序 30次数据分大小排好 取中间
				{
					for(k = j+1;k < 30;k++)
					{
						if(hrAvg_arg1[k] < hrAvg_arg1[j])
						{
							temp = hrAvg_arg1[j];
							hrAvg_arg1[j] = hrAvg_arg1[k];
							hrAvg_arg1[k] = temp;
						}
						if(spo2Avg_arg1[k] < spo2Avg_arg1[j])
						{
							temp = spo2Avg_arg1[j];
							spo2Avg_arg1[j] = spo2Avg_arg1[k];
							spo2Avg_arg1[k] = temp;
						}
					}
				}                        
				printf("%d %d\r\n",hrAvg_arg1[15],spo2Avg_arg1[15]); //hrAvg_arg1[15] 平均心率  spo2Avg_arg1[15]平均血氧   本人的串口
				j = 0;             //30次获取够了  让变量再为0 获取下次30次数据
			}
		}		
}
/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无	
 */

int main(void)
{
//	BEEP_Init();
	delay_init();	
	LED_Init();
	
	bsp_InitI2C();       //初始化IIC接口
	
	maxim_max30102_reset(); //resets the MAX30102
	maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_dummy); //Reads/clears the interrupt status register
 	maxim_max30102_init();  //initialize the MAX30102
	
	
	 //read the first 150 samples, and determine the signal range
	for(i = 0; i < n_ir_buffer_length; i++)//n_ir_buffer_length
	{
			maxim_max30102_read_fifo(&aun_red_buffer[i], &aun_ir_buffer[i]); //read from MAX30102 FIFO

			if(un_min > aun_red_buffer[i])
					un_min = aun_red_buffer[i]; //update signal min
			if(un_max < aun_red_buffer[i])
					un_max = aun_red_buffer[i]; //update signal max
	}
	un_prev_data = aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 150 samples (first 3 seconds of samples)
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

	
	/* 配置系统时钟为72M */
	SystemInit();
  
	DS18B20_Init();
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	OLED_ShowChinese(48,16,3,16,1);
	while(1)
	{
		LED_shanshuo();
		GPIO_SetBits(GPIOB,GPIO_Pin_8);  //给PB8高电平发声鸣叫
		delay_ms(500);
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);  //给PB8低电平停止鸣叫
		delay_ms(500);
		
		OLED_Refresh();
		Gain_Hr_Spo();//使用这个函数   spo2Avg_arg1[15]，hrAvg_arg1[15]就会有心率血氧值了
		TEMP_Value_Conversion();
		delay_ms(500);  
	}
}

