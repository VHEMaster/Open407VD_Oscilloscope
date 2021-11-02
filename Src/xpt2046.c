#include "stm32f4xx_hal.h"
#include "delay.h"
#include "xpt2046.h"

volatile uint16_t TP_X;
volatile uint16_t TP_Y;
volatile uint16_t TP_IRQ;

uint8_t TP_Check;
uint32_t TP_PressTime;

SPI_HandleTypeDef * xptspi;

#define size 100

uint32_t get(void)
{
	int16_t y_ret,x_ret = 0;
	static uint16_t XPT_TRANS[6] = {0x94,0,0xD4,0};
  static uint16_t XPT_REC[6];
	HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin,GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(xptspi, (uint8_t*)XPT_TRANS, (uint8_t*)XPT_REC, 4, 1000);
	HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin,GPIO_PIN_SET);

	x_ret = (XPT_REC[1]>>3)&0xFFF;
	if(x_ret<=249) x_ret = 0;
	else if(x_ret>=3423+249) x_ret = 319;
	else x_ret = (uint16_t)((float)(x_ret-249)/(float)10.7);


	y_ret = (XPT_REC[3]>>3)&0xFFF;
	if(y_ret>=3766) y_ret = 0;
	else if(y_ret<=382) y_ret = 239;
	else y_ret = (uint16_t)((float)(3740-y_ret)/(float)14.1);

	return (x_ret<<16)+y_ret;
}


void TP_Get(void)
{
	if(!HAL_GPIO_ReadPin(SPI2_IRQ_GPIO_Port, SPI2_IRQ_Pin))
	{
		if(TP_Check == 2)
		{
			check:
			TP_IRQ = 1;
			uint32_t tmp;
			uint32_t x_t = 0;
			uint32_t y_t = 0;
			uint8_t i = 0;
			for(i=0;i<size;i++)
			{
				tmp = get();
				x_t += (tmp>>16)&0xFFFF;
				y_t += tmp&0xFFFF;
			}

			TP_Y = y_t/size;
			TP_X = x_t/size;
			if(HAL_GPIO_ReadPin(SPI2_IRQ_GPIO_Port, SPI2_IRQ_Pin))
				TP_IRQ = 0;
		}
		else if(TP_Check == 1)
		{
			if((Delay_Tick-TP_PressTime)*DelayDivider >= 5000)
			{
				TP_Check = 2;
				goto check;
			}
		}
		else if(TP_Check == 0)
		{
			TP_PressTime = Delay_Tick;
			TP_Check = 1;
		}
	}
	else
	{
		TP_Check = 0;
		TP_IRQ = 0;
	}

}



void TP_Init(SPI_HandleTypeDef * hspi)
{
		xptspi = hspi;
	

	    TP_Check = 0;

}

