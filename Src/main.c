#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "gui.h"
#include "graph.h"
#include "delay.h"
#include "ili9325.h"
#include "xpt2046.h"
#include <string.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;

SRAM_HandleTypeDef hsram1;

osThreadId graphicTaskHandle;
osThreadId oscTaskHandle;

uint16_t DAC_SIN_TABLE[100] = 
{
	0x8000,0x8809,0x900a,0x97fc,0x9fd4,0xa78d,0xaf1e,0xb67f,
	0xbda9,0xc495,0xcb3c,0xd196,0xd79e,0xdd4e,0xe29f,0xe78d,
	0xec12,0xf02a,0xf3d0,0xf702,0xf9bb,0xfbfa,0xfdbb,0xfefd,
	0xffbe,0xffff,0xffbe,0xfefd,0xfdbb,0xfbfa,0xf9bb,0xf702,
	0xf3d0,0xf02a,0xec12,0xe78d,0xe29f,0xdd4e,0xd79e,0xd196,
	0xcb3c,0xc495,0xbda9,0xb67f,0xaf1e,0xa78d,0x9fd4,0x97fc,
	0x900a,0x8809,0x8000,0x77f6,0x6ff5,0x6803,0x602b,0x5872,
	0x50e1,0x4980,0x4256,0x3b6a,0x34c3,0x2e69,0x2861,0x22b1,
	0x1d60,0x1872,0x13ed,0xfd5,0xc2f,0x8fd,0x644,0x405,
	0x244,0x102,0x41,0x0,0x41,0x102,0x244,0x405,
	0x644,0x8fd,0xc2f,0xfd5,0x13ed,0x1872,0x1d60,0x22b1,
	0x2861,0x2e69,0x34c3,0x3b6a,0x4256,0x4980,0x50e1,0x5872,
	0x602b,0x6803,0x6ff5,0x77f6
};

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FSMC_Init(void);
static void MX_SPI2_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM8_Init(void);
static void MX_TIM3_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM7_Init(void);
void StartGraphicTask(void const * argument);
void StartOscTask(void const * argument);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


typedef enum 
{
	OSC_SR_1M = 1000000,
	OSC_SR_500k = 500000,
	OSC_SR_200k = 200000,
	OSC_SR_100k = 100000,
	OSC_SR_50k = 50000,
	OSC_SR_20k = 20000,
	OSC_SR_10k = 10000,
	OSC_SR_5k = 5000,
	OSC_SR_2k = 2000,
	OSC_SR_1k = 1000
}Osc_SampleRate;

typedef struct
{
	char * RangeValue;
	uint32_t RangeValueInUs;
	Osc_SampleRate SampleRate;
	float PixPerSample;
	float UsPerPix;
} Osc_PeriodDataTypeDef;

#define OSC_PIXPERDIV 30
#define OSC_PERIODCOUNT 14
#define OSC_RANGECOUNT 16

Osc_PeriodDataTypeDef Osc_PeriodData[OSC_PERIODCOUNT] = 
{
	{" 20us/d",20,     OSC_SR_1M,   0}, 
	{" 50us/d",50,     OSC_SR_1M,   0}, 
	{"100us/d",100,    OSC_SR_1M,   0}, 
	{"200us/d",200,    OSC_SR_1M,   0}, 
	{"500us/d",500,    OSC_SR_1M,   0}, 
	{"  1ms/d",1000,   OSC_SR_500k, 0}, 
	{"  2ms/d",2000,   OSC_SR_200k, 0}, 
	{"  5ms/d",5000,   OSC_SR_100k, 0}, 
	{" 10ms/d",10000,  OSC_SR_50k,  0}, 
	{" 20ms/d",20000,  OSC_SR_20k,  0}, 
	{" 50ms/d",50000,  OSC_SR_10k,  0}, 
	{"100ms/d",100000, OSC_SR_5k,   0}, 
	{"200ms/d",200000, OSC_SR_2k,   0}, 
	{"500ms/d",500000, OSC_SR_1k,   0}
};



typedef enum 
{
	OSC_RANGE_100V = 100000,
	OSC_RANGE_50V = 50000,
	OSC_RANGE_20V = 20000,
	OSC_RANGE_10V = 10000,
	OSC_RANGE_5V = 5000,
	OSC_RANGE_2V = 2000,
	OSC_RANGE_1V = 1000,
	OSC_RANGE_500m = 500,
	OSC_RANGE_200m = 200,
	OSC_RANGE_100m = 100,
	OSC_RANGE_50m = 50,
	OSC_RANGE_20m = 20,
	OSC_RANGE_10m = 10,
	OSC_RANGE_5m = 5,
	OSC_RANGE_2m = 2,
	OSC_RANGE_1m = 1
}Osc_Range;



const float Osc_ResistorDivider[8] = 
{
	2.0f,
	2.74074074f,
	7.91176471f,
	24.5f,
	40.1666667f,
	76.8064516f,
	214.636364f,
	471.0f
};


typedef struct
{
	char * RangeValue;
	Osc_Range Range;
	uint8_t ResistorDividerIndex;
	float ResistorDivider;
	float Divider;
	float RawDivider;
} Osc_RangeDataTypeDef;


Osc_RangeDataTypeDef Osc_RangeData[OSC_RANGECOUNT] = 
{
	{"100V/d ",   OSC_RANGE_100V, 7},
	{" 50V/d ",   OSC_RANGE_50V, 6},
	{" 20V/d ",   OSC_RANGE_20V, 5},
	{" 10V/d ",   OSC_RANGE_10V, 4},
	{"  5V/d ",   OSC_RANGE_5V, 3},
	{"  2V/d ",   OSC_RANGE_2V, 3},
	{"  1V/d ",   OSC_RANGE_1V, 2},
	{"500mV/d",   OSC_RANGE_500m, 1},
	{"200mV/d",   OSC_RANGE_200m, 0},
	{"100mV/d",   OSC_RANGE_100m, 0},
	{" 50mV/d",   OSC_RANGE_50m, 0},
	{" 20mV/d",   OSC_RANGE_20m, 0},
	{" 10mV/d",   OSC_RANGE_10m, 0},
	{"  5mV/d",   OSC_RANGE_5m, 0},
	{"  2mV/d",   OSC_RANGE_2m, 0},
	{"  1mV/d",   OSC_RANGE_1m, 0},
};


#define ADC_SIZE 10240

int16_t ADCD1[ADC_SIZE];
int16_t ADCD2[ADC_SIZE];

#define OSC_CHANNELS 2 

struct 
{
	float VoltageOffset[OSC_CHANNELS];
	int8_t Range[OSC_CHANNELS];
	uint8_t IsDC[OSC_CHANNELS];
	int32_t PeriodOffset;
	uint8_t Period;
	
} Osc_Settings;

uint16_t Osc_ChannelColor[OSC_CHANNELS] = { graph_color(255,255,0), graph_color(0,100,255) };

typedef struct
{
	ADC_HandleTypeDef * hadc;
	int16_t * ADC_DATA;
	float Divider;
	float OriginalDivider;
	float Min;
	float Max;
	float DC;
	float Freq;
	float Period;
	uint8_t ready;
	Osc_RangeDataTypeDef Range;
} Osc_ChannelDataTypeDef;

volatile uint8_t OSC_CH_READY = 0;

Osc_ChannelDataTypeDef Osc_ChannelData[OSC_CHANNELS];

void Osc_InitChannel(Osc_ChannelDataTypeDef * data)
{
	for(int i=0;i<OSC_CHANNELS;i++)
	{
		data[i].Divider = 1.0f / (65536.0f / 1.65f);
		data[i].OriginalDivider = 1.0f / (65536.0f / 1.65f);
		data[i].Min = 0.0f;
		data[i].Max = 0.0f;
		data[i].DC = 0.0f;
		data[i].ready = 0;
	}
}

typedef enum
{
	TRIGGERMODE_RISING = 0,
	TRIGGERMODE_FALLING,
	TRIGGERMODE_RISINGFALLING
} Osc_TriggerModeTypeDef;

struct
{
	float Voltage;
	Osc_TriggerModeTypeDef Mode;
	uint8_t Channel;
	int16_t TrigPoint;
} Osc_TriggerSettings;

#define Osc_GUIBGColor graph_color(0,50,150)


char * Osc_MainMenuItems[] = 
{
	//" Period", " Offset", "Range 1", "Range 2", "Offset 1", "Offset 2"
	" Period", "  Mode", "Range 1", "Range 2", "Offset 1", "Offset 2"
};
char Osc_MainMenuItemsValues[6][12];

uint8_t Osc_MainMenuData[5] = { 0, 4, 6, 1, 10 };

char * Osc_MainMenuTitles[] = 
{
	"Avg CH1", "Avg CH2", "Min CH1", "Min CH2", "Max CH1", "Max CH2", "FreqCH1", "FreqCH2", "Prd CH1", "Prd CH2", "TrigCH1", "TrigCH2"
};
char Osc_MainMenuValues[12][12];


Osc_PeriodDataTypeDef Osc_CurrentPeriod;

uint32_t ADC_PERIOD = 0;
		static int32_t adc_per = 0;


void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		adc_per = Delay_Tick;
	}
}

float ADC_OFFSET[2] = { 5390, 4980};


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	uint8_t ch;
	if(hadc->Instance == ADC1)
	{
		ADC_PERIOD = (Delay_Tick - adc_per)*2;
		ch = 0;
	}
	else if(hadc->Instance == ADC2)
	{
		ch = 1;
	}
	else return;
	
	int32_t RAW = 0;
	int16_t MIN = 32767, MAX = -32768;
	int16_t * data = Osc_ChannelData[ch].ADC_DATA;
	for(int i=0;i<ADC_SIZE;i++)
	{
		*data ^= 0x8000;
		*data += ADC_OFFSET[ch];
		if(*data < MIN) MIN = *data;
		if(*data > MAX) MAX = *data;
		RAW += *data;
		data++;
	}
	if(ch == Osc_TriggerSettings.Channel)
	{
		Osc_TriggerSettings.TrigPoint = ADC_SIZE>>1;
		//if(Osc_TriggerSettings.Mode != TRIGGERMODE_OFF)
		{
			int32_t TRIGLEVEL = (Osc_TriggerSettings.Voltage / Osc_ChannelData[ch].Divider);
			int16_t dat_left,dat_right,dat_left1,dat_right1;
			uint16_t pos;
			for(int i=25;i<((ADC_SIZE>>1)-25);i++)
			{
				pos = (ADC_SIZE>>1)-i;
				dat_left = Osc_ChannelData[ch].ADC_DATA[pos];
				dat_left1 = Osc_ChannelData[ch].ADC_DATA[pos-20];
				if(Osc_TriggerSettings.Mode == TRIGGERMODE_RISING || Osc_TriggerSettings.Mode == TRIGGERMODE_RISINGFALLING)
				{
					if(dat_left-0 > TRIGLEVEL && dat_left1+0 < TRIGLEVEL) 
					{
						Osc_TriggerSettings.TrigPoint = pos;
						break;
					}
				}
				if(Osc_TriggerSettings.Mode == TRIGGERMODE_FALLING || Osc_TriggerSettings.Mode == TRIGGERMODE_RISINGFALLING)
				{
					if(dat_left+0 < TRIGLEVEL && dat_left1-0 > TRIGLEVEL) 
					{
						Osc_TriggerSettings.TrigPoint = pos;
						break;
					}
				}
				
				pos = (ADC_SIZE>>1)+i;
				dat_right = Osc_ChannelData[ch].ADC_DATA[pos];
				dat_right1 = Osc_ChannelData[ch].ADC_DATA[pos+20];
				if(Osc_TriggerSettings.Mode == TRIGGERMODE_RISING || Osc_TriggerSettings.Mode == TRIGGERMODE_RISINGFALLING)
				{
					if(dat_right+0 < TRIGLEVEL && dat_right1-0 > TRIGLEVEL) 
					{
						Osc_TriggerSettings.TrigPoint = pos;
						break;
					}
				}
				if(Osc_TriggerSettings.Mode == TRIGGERMODE_FALLING || Osc_TriggerSettings.Mode == TRIGGERMODE_RISINGFALLING)
				{
					if(dat_right-0 > TRIGLEVEL && dat_right1+0 < TRIGLEVEL)
					{
						Osc_TriggerSettings.TrigPoint = pos;
						break;
					}
				}
			}
		}
	}
	
	Osc_ChannelData[ch].Min = MIN * Osc_ChannelData[ch].Divider;
	Osc_ChannelData[ch].Max = MAX * Osc_ChannelData[ch].Divider;
	Osc_ChannelData[ch].DC = ((float)(RAW/ADC_SIZE)) * Osc_ChannelData[ch].Divider;
	
	int8_t rising_nfaiiling = 0;
	int16_t last = 0;
	int32_t raw = Osc_TriggerSettings.Voltage / Osc_ChannelData[ch].Divider;
	float distances = 0;
	int16_t iterations = -1;
	float period;
	data = Osc_ChannelData[ch].ADC_DATA;
	for(int i=0;i<ADC_SIZE;i++)
	{
		if(rising_nfaiiling)
		{
			if((data[i])>raw) 
			{
				rising_nfaiiling = 0;
				if(iterations >= 0)
					distances += i - last;
				iterations++;
				last = i;
			}
		}
		else
		{
			if((data[i])<raw) 
			{
				rising_nfaiiling = 1;
				if(iterations >= 0)
					distances += i - last;
				iterations++;
				last = i;
			}
		}
	}
	distances /= iterations;
	distances *= 2;
	period = distances/Osc_CurrentPeriod.SampleRate;
	
	if(period > 0)
	{
		Osc_ChannelData[ch].Period = period;
		Osc_ChannelData[ch].Freq = 1.0f / Osc_ChannelData[ch].Period;
		Osc_ChannelData[ch].Period *= 1000000.0f;
	}
	
	Osc_ChannelData[ch].ready = 1;
	
	
		
	if(Osc_ChannelData[0].ready && Osc_ChannelData[1].ready) 
		OSC_CH_READY = 1;
}


void Osc_InitSamplesPerPix(Osc_PeriodDataTypeDef * data)
{
	for(int i=0;i<OSC_PERIODCOUNT;i++)
	{
		data[i].PixPerSample = (float)OSC_PIXPERDIV/((data[i].RangeValueInUs*0.000001f)/(1.0f/(float)data[i].SampleRate));
		data[i].UsPerPix =  ((1.0f/(float)data[i].SampleRate) / (float)data[i].PixPerSample) * 1000000.0f;
	}
}

void Osc_SetDividerPhyValue(uint8_t channel, uint8_t value)
{
	if(channel == 0)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); //GND
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, value & 1 ? GPIO_PIN_RESET : GPIO_PIN_SET); //S0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, value & 2 ? GPIO_PIN_RESET : GPIO_PIN_SET); //S1
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, value & 4 ? GPIO_PIN_RESET : GPIO_PIN_SET); //S2
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET); //GND
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, value & 1 ? GPIO_PIN_RESET : GPIO_PIN_SET); //S0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, value & 2 ? GPIO_PIN_RESET : GPIO_PIN_SET); //S1
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, value & 4 ? GPIO_PIN_RESET : GPIO_PIN_SET); //S2
	}
}

void Osc_InitRangeRawData(Osc_RangeDataTypeDef * data)
{
	for(int i=0;i<OSC_RANGECOUNT;i++)
	{
		data[i].ResistorDivider = Osc_ResistorDivider[data[i].ResistorDividerIndex];
		data[i].RawDivider = (1.0f / ((65536.0f / 3.3f) / (float)OSC_PIXPERDIV * 0.001f * (float)data[i].Range)) * data[i].ResistorDivider;
		data[i].Divider = ((1.0f/(0.001f * (float)data[i].Range)) * (float)OSC_PIXPERDIV) * data[i].ResistorDivider;
	}
}

void Osc_SetSamplerate(Osc_SampleRate rate)
{
	uint32_t TIMFreq = HAL_RCC_GetPCLK2Freq();
	int32_t PSC_Div;
	int32_t ARR_Div;
	TIM8->CNT = 0;
	
	switch(rate)
	{
		case OSC_SR_1M : 
			PSC_Div = 1; 
			ARR_Div = 83 * 1.428572f; 
			break;
		case OSC_SR_500k : 
			PSC_Div = 1; 
			ARR_Div = 167 * 1.428572f;
			break;
		case OSC_SR_200k : 
			PSC_Div = 1; 
			ARR_Div = 419 * 1.428572f;
			break;
		case OSC_SR_100k : 
			PSC_Div = 1; 
			ARR_Div = 839 * 1.428572f;
			break;
		case OSC_SR_50k : 
			PSC_Div = 1; 
			ARR_Div = 1679 * 1.428572f;
			break;
		case OSC_SR_20k : 
			PSC_Div = 1; 
			ARR_Div = 4199 * 1.428572f;
			break;
		case OSC_SR_10k : 
			PSC_Div = 19; 
			ARR_Div = 839 * 1.428572f;
			break;
		case OSC_SR_5k : 
			PSC_Div = 19; 
			ARR_Div = 1679 * 1.428572f;
			break;
		case OSC_SR_2k : 
			PSC_Div = 19; 
			ARR_Div = 4199 * 1.428572f;
			break;
		case OSC_SR_1k : 
			PSC_Div = 39; 
			ARR_Div = 4199 * 1.428572f;
			break;
	}
	
	TIM8->PSC = PSC_Div;
	TIM8->ARR = ARR_Div;
}

void Osc_SetTrigger(float level, uint8_t channel, Osc_TriggerModeTypeDef mode)
{
	
	Osc_MainMenuData[4] = 10+channel;
	sprintf(Osc_MainMenuValues[Osc_MainMenuData[4]], "% .2fV",level);
	
	Osc_TriggerSettings.Channel = channel;
	Osc_TriggerSettings.Voltage = level;
	Osc_TriggerSettings.Mode = mode;
	
}

void Osc_SetPeriod(uint8_t range)
{
	if(range < OSC_PERIODCOUNT)
	{
		Osc_CurrentPeriod = Osc_PeriodData[range];
		Osc_SetSamplerate(Osc_CurrentPeriod.SampleRate);
		strcpy(Osc_MainMenuItemsValues[0],Osc_CurrentPeriod.RangeValue);
		
		Osc_Settings.Period = range;
	}
}


void Osc_SetRange(uint8_t range, uint8_t channel)
{
	if(range < OSC_RANGECOUNT)
	{
		Osc_SetDividerPhyValue(channel,  Osc_RangeData[range].ResistorDividerIndex);
		Osc_ChannelData[channel].Range = Osc_RangeData[range];
		Osc_ChannelData[channel].Divider = Osc_ChannelData[channel].OriginalDivider * Osc_RangeData[range].ResistorDivider;
		strcpy(Osc_MainMenuItemsValues[2+channel],Osc_RangeData[range].RangeValue);
		Osc_Settings.Range[channel] = range;
	}
}


float Osc_VoltageOffsets[] = { 0.0f, 0.01f, 0.02f, 0.05f, 0.1f, 0.2f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f, 20.0f };
int32_t Osc_PeriodOffsets[] = { 0,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000 };

void Osc_SetVoltageOffset(float offset, uint8_t channel)
{
	Osc_Settings.VoltageOffset[channel] = offset;
	sprintf(Osc_MainMenuItemsValues[4+channel], offset<1.0f&&offset>-1.0f?"%-4.0fmV ":"%-6.2fV ", offset<1.0f&&offset>-1.0f?offset*1000.0f:offset);
	
}
void Osc_SetPeriodOffset(int32_t offset)
{
	Osc_Settings.PeriodOffset = offset;
	sprintf(Osc_MainMenuItemsValues[1], (offset<1000 && offset>-1000)?"%-4dus ":"%-4dms ", (offset<1000 && offset>-1000)?(offset):(offset/1000));
}


void Osc_SetAcDcMode(uint8_t mode)
{
	Osc_Settings.IsDC[0] = mode & 1;
	Osc_Settings.IsDC[1] = mode & 2;
	sprintf(Osc_MainMenuItemsValues[1], " %s/%s", (mode & 1 ? "DC" : "AC") , (mode & 2 ? "DC" : "AC"));
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, mode & 1 ? GPIO_PIN_SET : GPIO_PIN_RESET); //nAC
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, mode & 2 ? GPIO_PIN_SET : GPIO_PIN_RESET); //nAC
}

void Osc_RepaintText(void )
{
	for(int i=0;i<6;i++)
	{
		graph_print_bg(271,28+(i*32),0xFFFF,Osc_GUIBGColor,1,Osc_MainMenuItems[i]);
		graph_print_bg(271,38+(i*32),0xFFFF,Osc_GUIBGColor,1,Osc_MainMenuItemsValues[i]);
	}
	
	for(int i=0;i<5;i++)
	{
		graph_print_bg(6+(i*64),218,0xFFFF,Osc_GUIBGColor,1,Osc_MainMenuTitles[Osc_MainMenuData[i]]);
		graph_print_bg(6+(i*64),229,0xFFFF,Osc_GUIBGColor,1,Osc_MainMenuValues[Osc_MainMenuData[i]]);
	}
}

void Osc_RepaintFull()
{
	graph_clear();
	graph_square(0xFFFF,0,0,269,214);
	graph_square_bg(0xFFFF,Osc_GUIBGColor,269,0,319,22);
	graph_print(278,4,0xFFFF,2,"OSC");
	
	for(int i=0;i<6;i++)
	{
		graph_square_bg(0xFFFF,Osc_GUIBGColor,269,22+(i*32),319,22+32+(i*32));
	}
	
	for(int i=0;i<5;i++)
	{
		graph_square_bg(0xFFFF,Osc_GUIBGColor,i*64,214,63+(i*64),239);
	}
	Osc_RepaintText();
}



struct
{
	uint8_t Period;
	uint8_t Offset;
	uint8_t Range1;
	uint8_t Range2;
	uint8_t Offset1;
	uint8_t Offset2;
	
	uint8_t Item1;
	uint8_t Item2;
	uint8_t Item3;
	uint8_t Item4;
	uint8_t Trigger;
	
	uint8_t Plus;
	uint8_t Minus;
	uint8_t Next;
	uint8_t Prev;
	
	uint8_t Up;
	uint8_t Down;
} Osc_Buttons;

typedef enum
{
	MENU_MAIN = 0,
	MENU_PERIOD,
	MENU_OFFSET,
	MENU_RANGE1,
	MENU_RANGE2,
	MENU_OFFSET1,
	MENU_OFFSET2,
	MENU_TRIGGER,
	MENU_ITEM1,
	MENU_ITEM2,
	MENU_ITEM3,
	MENU_ITEM4
} Osc_MenuTypeDef;

Osc_MenuTypeDef CurrentMenu = MENU_MAIN;

void Osc_PressButtonCallBack(uint8_t id)
{
	if(id == Osc_Buttons.Period) { if(CurrentMenu == MENU_PERIOD) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_PERIOD; }
	else if(id == Osc_Buttons.Offset) { if(CurrentMenu == MENU_OFFSET) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_OFFSET; }
	else if(id == Osc_Buttons.Range1) { if(CurrentMenu == MENU_RANGE1) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_RANGE1; }
	else if(id == Osc_Buttons.Range2) { if(CurrentMenu == MENU_RANGE2) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_RANGE2; }
	else if(id == Osc_Buttons.Offset1) { if(CurrentMenu == MENU_OFFSET1) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_OFFSET1; }
	else if(id == Osc_Buttons.Offset2) { if(CurrentMenu == MENU_OFFSET2) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_OFFSET2; }
	else if(id == Osc_Buttons.Trigger) { if(CurrentMenu == MENU_TRIGGER) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_TRIGGER; }
	else if(id == Osc_Buttons.Item1) { if(CurrentMenu == MENU_ITEM1) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_ITEM1; }
	else if(id == Osc_Buttons.Item2) { if(CurrentMenu == MENU_ITEM2) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_ITEM2; }
	else if(id == Osc_Buttons.Item3) { if(CurrentMenu == MENU_ITEM3) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_ITEM3; }
	else if(id == Osc_Buttons.Item4) { if(CurrentMenu == MENU_ITEM4) CurrentMenu = MENU_MAIN; else CurrentMenu = MENU_ITEM4; }
}

void Osc_ADCReset(void)
{
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Stop_DMA(&hadc2);
	Osc_ChannelData[0].ready = 0;
	Osc_ChannelData[1].ready = 0;
	OSC_CH_READY = 0;
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)Osc_ChannelData[0].ADC_DATA, ADC_SIZE);
	HAL_ADC_Start_DMA(&hadc2,(uint32_t*)Osc_ChannelData[1].ADC_DATA, ADC_SIZE);
}

void StartGraphicTask(void const * argument)
{
	int i;
	Osc_MenuTypeDef lastmenu = MENU_MAIN;
	uint8_t currentoffset1= 0;
	uint8_t currentoffset2 = 0;
	uint8_t currentperiodoffset = 0;
	uint8_t currentrange1 = 6;
	uint8_t currentrange2 = 6;
	uint8_t currentperiod = 4;
	
	uint16_t ScrX1,ScrX2,ScrY1,ScrY2;
	
	ScrX1 = 0;
	ScrX2 = 0;
	ScrY1 = 0;
	ScrY2 = 0;
	
	Osc_InitSamplesPerPix(Osc_PeriodData);
	Osc_InitRangeRawData(Osc_RangeData);
	Osc_SetPeriod(currentperiod);
	Osc_SetTrigger(0.0f, 0, TRIGGERMODE_RISING);
	Osc_SetRange(currentrange1,0);
	Osc_SetRange(currentrange2,1);
	
	Osc_SetPeriodOffset(0);
	Osc_SetAcDcMode(0);
	Osc_SetVoltageOffset(0, 0);
	Osc_SetVoltageOffset(0, 1);
	
	
	TP_Init(&hspi2);
	graph_init();
	GUI_Init();
	
	
	graph_square(0xFFFF,40,40,280,140);
	graph_print(50,55,0xFFFF,3, "Oscilloscope");
	graph_print(50,83,0xFFFF,1, "Based on Open407V-D");
	graph_print(110,92,0xFFFF,1, "STM32F407VG");
	
	DelayMs(1000);
	
	Osc_RepaintFull();
	
	uint32_t TimeLastUpdated = HAL_GetTick();
	
	
	Osc_Buttons.Period = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,269,22+(0*32),319,22+32+(0*32));
	Osc_Buttons.Offset = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,269,22+(1*32),319,22+32+(1*32));
	Osc_Buttons.Range1 = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,269,22+(2*32),319,22+32+(2*32));
	Osc_Buttons.Range2 = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,269,22+(3*32),319,22+32+(3*32));
	Osc_Buttons.Offset1 = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,269,22+(4*32),319,22+32+(4*32));
	Osc_Buttons.Offset2 = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,269,22+(5*32),319,22+32+(5*32));
	
	Osc_Buttons.Item1 = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,0*64,214,63+(0*64),239);
	Osc_Buttons.Item2 = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,1*64,214,63+(1*64),239);
	Osc_Buttons.Item3 = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,2*64,214,63+(2*64),239);
	Osc_Buttons.Item4 = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,3*64,214,63+(3*64),239);
	Osc_Buttons.Trigger = GUI_CreateButton((uint32_t)&Osc_PressButtonCallBack,NULL,NULL,4*64,214,63+(4*64),239);
	
  for(;;)
  {
		TP_Get();
		GUI_CallDoPress(TP_X,TP_Y,TP_IRQ);
		/*
		if(TP_IRQ)
		{
			graph_print_bg(10,10,0xFFFF,0,1,"X:% 4d  Y:% 4d",TP_X, TP_Y);
			LCD_IO_XY(TP_Y,TP_X);
			LCD_IO_DAT(0xFFFF);
		}
		*/
		
		
		if(CurrentMenu == MENU_ITEM1)
		{
			i = 0;
			Osc_MainMenuData[i]++;
			if(Osc_MainMenuData[i] >= 10) Osc_MainMenuData[i] = 0;
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		else if(CurrentMenu == MENU_ITEM2)
		{
			i = 1;
			Osc_MainMenuData[i]++;
			if(Osc_MainMenuData[i] >= 10) Osc_MainMenuData[i] = 0;
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		else if(CurrentMenu == MENU_ITEM3)
		{
			i = 2;
			Osc_MainMenuData[i]++;
			if(Osc_MainMenuData[i] >= 10) Osc_MainMenuData[i] = 0;
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		else if(CurrentMenu == MENU_ITEM4)
		{
			i = 3;
			Osc_MainMenuData[i]++;
			if(Osc_MainMenuData[i] >= 10) Osc_MainMenuData[i] = 0;
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		else if(CurrentMenu == MENU_TRIGGER)
		{
			i = 4;
			Osc_MainMenuData[i]++;
			if(Osc_MainMenuData[i] >= 12) Osc_MainMenuData[i] = 10;
			//Osc_SetTrigger(Osc_ChannelData[Osc_MainMenuData[i]-10].DC,Osc_MainMenuData[i]-10,TRIGGERMODE_RISING);
			Osc_SetTrigger((Osc_ChannelData[Osc_MainMenuData[i]-10].Min+Osc_ChannelData[Osc_MainMenuData[i]-10].Max)*0.5f,Osc_MainMenuData[i]-10,TRIGGERMODE_RISING);
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		
		else if(CurrentMenu == MENU_PERIOD)
		{
			currentperiod++;
			Osc_SetPeriod(currentperiod<(sizeof(Osc_PeriodData)/sizeof(Osc_PeriodDataTypeDef))?currentperiod:(currentperiod=0));
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		
		else if(CurrentMenu == MENU_RANGE1)
		{
			currentrange1++;
			Osc_SetRange(currentrange1<(sizeof(Osc_RangeData)/sizeof(Osc_RangeDataTypeDef))?currentrange1:(currentrange1=0),0);
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		else if(CurrentMenu == MENU_RANGE2)
		{
			currentrange2++;
			Osc_SetRange(currentrange2<(sizeof(Osc_RangeData)/sizeof(Osc_RangeDataTypeDef))?currentrange2:(currentrange2=0),1);
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		
		else if(CurrentMenu == MENU_OFFSET1)
		{
			currentoffset1++;
		  Osc_SetVoltageOffset(currentoffset1 == 0 ? (Osc_VoltageOffsets[0]) : Osc_VoltageOffsets[((currentoffset1-1)/2)<(sizeof(Osc_VoltageOffsets)/sizeof(float))?((currentoffset1-1)/2):(currentoffset1=0)]*(((currentoffset1-1)%2)==0?1.0f:(0.0f-1.0f)),0);
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		else if(CurrentMenu == MENU_OFFSET2)
		{
			currentoffset2++;
		  Osc_SetVoltageOffset(currentoffset2 == 0 ? (Osc_VoltageOffsets[0]) : Osc_VoltageOffsets[((currentoffset2-1)/2)<(sizeof(Osc_VoltageOffsets)/sizeof(float))?((currentoffset2-1)/2):(currentoffset2=0)]*(((currentoffset2-1)%2)==0?1.0f:(0.0f-1.0f)),1);
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		else if(CurrentMenu == MENU_OFFSET)
		{
			/*
			currentperiodoffset++;
		  Osc_SetPeriodOffset(currentperiodoffset == 0 ? (Osc_PeriodOffsets[0]) : Osc_PeriodOffsets[((currentperiodoffset-1)/2)<(sizeof(Osc_PeriodOffsets)/sizeof(int32_t))?((currentperiodoffset-1)/2):(currentperiodoffset=0)]*(((currentperiodoffset-1)%2)==0?1.0f:(0.0f-1.0f)));
			*/
			Osc_SetAcDcMode(++currentperiodoffset);
			Osc_RepaintText();
			CurrentMenu = MENU_MAIN;
		}
		
		if(OSC_CH_READY) 
		{
			OSC_CH_READY = 0;
		
			if(CurrentMenu == MENU_MAIN)
			{
				for(int i=1;i<214;i++)
				{
					LCD_IO_XY(i,1);
					for(int j=1;j<269;j++)
					{
						if((107-i)%OSC_PIXPERDIV == 0 || (j-1)%OSC_PIXPERDIV == 0)
						{
							LCD_IO_DAT(graph_color(64,64,64));
						}
						else LCD_IO_DAT(0);
					}
				}
			}
			else
			{
				uint8_t needupd;
				for(int i=1;i<214;i++)
				{
					needupd = 0;
					LCD_IO_XY(i,1);
					for(int j=1;j<269;j++)
					{
						if(j<ScrX1 && j>ScrX2 && i<ScrY1 && i>ScrY2)
						{
							if(needupd)
							{
								needupd=0;
								LCD_IO_XY(i,j);
							}
							
							if((107-i)%OSC_PIXPERDIV == 0 || (j-1)%OSC_PIXPERDIV == 0)
							{
								LCD_IO_DAT(graph_color(64,64,64));
							}
							else LCD_IO_DAT(0);
						}
						else needupd = 1;
					}
				}
			}
			
			for(int ch=0;ch<OSC_CHANNELS;ch++)
			{
				graph_print(5,5+(ch*9),Osc_ChannelColor[ch],1,"CH%d: % .2fV  ", ch+1, Osc_ChannelData[ch].DC);
			}
			
			for(int ch=0;ch<OSC_CHANNELS;ch++)
			{
				uint16_t color = Osc_ChannelColor[ch];
				{
					int32_t pointer = Osc_TriggerSettings.TrigPoint - ((0.0f+(Osc_Settings.PeriodOffset * (1.0f/Osc_CurrentPeriod.UsPerPix)))/Osc_CurrentPeriod.PixPerSample);
					float x,y,startx,starty;
					x = 0;
					y = 0;
					
					startx = 1.0f;
					if(pointer >= 0 && pointer < ADC_SIZE)
						starty = 107-((float)(Osc_ChannelData[ch].ADC_DATA[pointer]) * Osc_ChannelData[ch].Range.RawDivider);
					
					for(x=1.0f;x<269;)
					{
						x += Osc_CurrentPeriod.PixPerSample;
						if(pointer >= 0 && pointer < ADC_SIZE)
							y = 107-(((float)(Osc_ChannelData[ch].ADC_DATA[pointer]) * Osc_ChannelData[ch].Range.RawDivider) + Osc_Settings.VoltageOffset[ch] * Osc_ChannelData[ch].Range.Divider);
						
						if(CurrentMenu == MENU_MAIN)
							graph_line_limited(startx,starty,x,y,1,1,268,213,color);
						else graph_line_limited2(startx,starty,x,y,1,1,268,213,ScrX1,ScrY1,ScrX2,ScrY2,color);
							
						
						startx = x;
						starty = y;
						
						pointer++;
					}
					Osc_ChannelData[ch].ready = 0;
				}
			}
			HAL_ADC_Start_DMA(&hadc1,(uint32_t*)Osc_ChannelData[0].ADC_DATA, ADC_SIZE);
			HAL_ADC_Start_DMA(&hadc2,(uint32_t*)Osc_ChannelData[1].ADC_DATA, ADC_SIZE);
			if(HAL_GetTick()-TimeLastUpdated > 500)
			{
				//"Avg CH1", "Avg CH2", "Min CH1", "Min CH2", "Max CH1", "Max CH2", "TrigCH1", "TrigCH2"
			  float freq1 = Osc_ChannelData[0].Freq;
				float freq2 = Osc_ChannelData[1].Freq;
				float per1 = Osc_ChannelData[0].Period;
				float per2 = Osc_ChannelData[1].Period;
				for(int i=0;i<12;i++)
				{
					switch(i)			
					{			
						case 0 :
							sprintf(Osc_MainMenuValues[i],"% 6.2fV ", Osc_ChannelData[0].DC);
							break;
						case 1 :
							sprintf(Osc_MainMenuValues[i],"% 6.2fV ", Osc_ChannelData[1].DC);
							break;
						case 2 :
							sprintf(Osc_MainMenuValues[i],"% 6.2fV ", Osc_ChannelData[0].Min);
							break;
						case 3 :
							sprintf(Osc_MainMenuValues[i],"% 6.2fV ", Osc_ChannelData[1].Min);
							break;
						case 4 :
							sprintf(Osc_MainMenuValues[i],"% 6.2fV ", Osc_ChannelData[0].Max);
							break; 
						case 5 :
							sprintf(Osc_MainMenuValues[i],"% 6.2fV ", Osc_ChannelData[1].Max);
							break;
						case 6 :
							if(freq1 > 100000.0f)
								sprintf(Osc_MainMenuValues[i], "-------  ");
							else sprintf(Osc_MainMenuValues[i],(freq1<5000.0f)?"%-5.0fHz ":"%-5.1fkHz ", (freq1<5000.0f)?freq1:freq1*0.001f);
							break;
						case 7 :
							if(freq2 > 100000.0f) 
								sprintf(Osc_MainMenuValues[i], "-------  ");
							else sprintf(Osc_MainMenuValues[i],(freq2<5000.0f)?"%-5.0fHz ":"%-5.1fkHz ", (freq2<5000.0f)?freq2:freq2*0.001f);
							break;
						case 8 :
							if(freq1 > 100000.0f)
								sprintf(Osc_MainMenuValues[i], "-------  ");
							else sprintf(Osc_MainMenuValues[i],(per1<5000.0f)?"%-5.0fus ":"%-5.1fms ", (per1<5000.0f)?per1:per1*0.001f);
							break;
						case 9 :
							if(freq2 > 100000.0f)
								sprintf(Osc_MainMenuValues[i], "-------  ");
							else sprintf(Osc_MainMenuValues[i],(per2<5000.0f)?"%-5.0fus ":"%-5.1fms ", (per2<5000.0f)?per2:per2*0.001f);
							break;
						case 10 : case 11 :
							sprintf(Osc_MainMenuValues[i],"% .2fV", Osc_TriggerSettings.Voltage);
							break;
						default: strcpy(Osc_MainMenuValues[i]," N/A "); break;
					}						
				}
				Osc_RepaintText();
				TimeLastUpdated = HAL_GetTick();
			}
			osDelay(50);
		}
		osDelay(5);
  }
  /* USER CODE END 5 */ 
}
/* USER CODE END 0 */
/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM8_Init();
  MX_TIM3_Init();
  MX_DAC_Init();
  MX_TIM7_Init();
	DelayInit();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of graphicTask */
  osThreadDef(graphicTask, StartGraphicTask, osPriorityNormal, 0, 128);
  graphicTaskHandle = osThreadCreate(osThread(graphicTask), NULL);

	
	//Start ADC
	HAL_TIM_Base_Start(&htim8);
	Osc_ChannelData[0].ADC_DATA = ADCD1;
	Osc_ChannelData[1].ADC_DATA = ADCD2;
	Osc_ChannelData[0].hadc = &hadc1;
	Osc_ChannelData[1].hadc = &hadc2;
	Osc_InitChannel(Osc_ChannelData);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)Osc_ChannelData[0].ADC_DATA, ADC_SIZE);
	HAL_ADC_Start_DMA(&hadc2,(uint32_t*)Osc_ChannelData[1].ADC_DATA, ADC_SIZE);
	
	//Start DAC as 1KHz Sin output
	HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1, (uint32_t*)DAC_SIN_TABLE, 100, DAC_ALIGN_12B_L);
	HAL_TIM_Base_Start(&htim7);
	
	//Start PWM as 1KHz Square output
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 //StartGraphicTask(NULL);

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_LEFT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* ADC2 init function */
static void MX_ADC2_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc2.Init.DataAlign = ADC_DATAALIGN_LEFT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* DAC init function */
static void MX_DAC_Init(void)
{

  DAC_ChannelConfTypeDef sConfig;

    /**DAC Initialization 
    */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**DAC channel OUT1 config 
    */
  sConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 119;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* TIM7 init function */
static void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 119;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 9;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM8 init function */
static void MX_TIM8_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 83;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
	//We dont need IRQ from DAC
  //HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  //HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED_GREEN_Pin|LED_ORANGE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUT_USER_Pin */
	/*
  GPIO_InitStruct.Pin = BUT_USER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(BUT_USER_GPIO_Port, &GPIO_InitStruct);
	*/

  /*Configure GPIO pin : SPI2_NSS_Pin */
  GPIO_InitStruct.Pin = SPI2_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SPI2_NSS_GPIO_Port, &GPIO_InitStruct);
	
	
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_14|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI2_IRQ_Pin */
  GPIO_InitStruct.Pin = SPI2_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SPI2_IRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_GREEN_Pin LED_ORANGE_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin|LED_ORANGE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{
  FSMC_NORSRAM_TimingTypeDef Timing;

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 0;
  Timing.DataSetupTime = 8;
  Timing.BusTurnAroundDuration = 0;
  Timing.CLKDivision = 0;
  Timing.DataLatency = 0;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
