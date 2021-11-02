#include "stm32f4xx_hal.h"
#include "delay.h"


extern volatile uint16_t TP_X;
extern volatile uint16_t TP_Y;
extern volatile uint16_t TP_IRQ;

extern uint8_t TP_Check;
extern uint32_t TP_PressTime;

void TP_Get(void);
void TP_Init(SPI_HandleTypeDef * hspi);
