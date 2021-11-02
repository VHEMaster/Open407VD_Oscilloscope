#include "stm32f4xx.h"
#include "graph.h"
#include "ili9325.h"

#define TRIGGERS 64

typedef struct
{
	uint16_t	x1;
	uint16_t	x2;
	uint16_t	y1;
	uint16_t	y2;
	uint32_t	doPress;
	uint32_t	doOver;
	uint32_t	doPoint;
}TButton;

typedef struct
{
	uint8_t		enabled[TRIGGERS];
	TButton 	Trigger[TRIGGERS];
}RegisteredTriggers;

extern RegisteredTriggers GUI_Triggers;



void GUI_Init(void);
void GUI_CallDoPress(uint16_t x, uint16_t y, uint8_t iqr);
int8_t GUI_CreateButton(uint32_t onselect, uint32_t onpoint, uint32_t onover, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
uint8_t GUI_RemoveButton(uint8_t ID);
