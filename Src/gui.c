#include "stm32f4xx.h"
#include "gui.h"

uint16_t GUI_FX;
uint16_t GUI_FY;
uint8_t GUI_FT;
uint8_t GUI_CT;
uint8_t GUI_EX;
uint8_t GUI_Pointed;
RegisteredTriggers GUI_Triggers;

void GUI_Clear(void)
{
	for(int i=0;i<TRIGGERS;i++)
		GUI_Triggers.enabled[i] = 0;
}

void GUI_Init(void)
{
	uint8_t i;
	for(i=0;i<TRIGGERS;i++)
		GUI_Triggers.enabled[i] = 0;
	GUI_FT = 1;
	GUI_CT = 0;
	GUI_EX = 0;
	GUI_Pointed = 255;
}

uint8_t GUI_RemoveButton(uint8_t ID)
{
	if(GUI_Triggers.enabled[ID] == 0)
		return 1;
	else GUI_Triggers.enabled[ID] = 0;
	return 0;
}

void GUI_CallDoPress(uint16_t x, uint16_t y, uint8_t irq)
{
	uint8_t i;
	uint32_t (*onPress)(uint8_t);
	uint32_t (*onOver)(uint8_t);
	uint32_t (*onPoint)(uint8_t);
	if(irq == 1)
	{
		if(GUI_FT == 1)
		{
			if(GUI_CT == 0)
			{
				GUI_FX = x;
				GUI_FY = y;
				GUI_FT = 0;

				for(i=0;i<TRIGGERS;i++)
					if(GUI_Triggers.enabled[i] == 1)
						if(x>GUI_Triggers.Trigger[i].x1 && x<GUI_Triggers.Trigger[i].x2 && y>GUI_Triggers.Trigger[i].y1 && y<GUI_Triggers.Trigger[i].y2 && GUI_FX>GUI_Triggers.Trigger[i].x1 && GUI_FX<GUI_Triggers.Trigger[i].x2 && GUI_FY>GUI_Triggers.Trigger[i].y1 && GUI_FY<GUI_Triggers.Trigger[i].y2)
						{
							GUI_Pointed = i;
							if(GUI_Triggers.Trigger[i].doPoint != 0)
							{
								onPoint = (uint32_t (*)(uint8_t))GUI_Triggers.Trigger[i].doPoint;
								onPoint(i);
							}
						}

			}
			else GUI_CT--;
		}
	}
	else
	{
		if(GUI_FT == 0)
		{
			for(i=0;i<TRIGGERS;i++)
				if(GUI_Triggers.enabled[i] == 1)
					if(x>GUI_Triggers.Trigger[i].x1 && x<GUI_Triggers.Trigger[i].x2 && y>GUI_Triggers.Trigger[i].y1 && y<GUI_Triggers.Trigger[i].y2 && GUI_FX>GUI_Triggers.Trigger[i].x1 && GUI_FX<GUI_Triggers.Trigger[i].x2 && GUI_FY>GUI_Triggers.Trigger[i].y1 && GUI_FY<GUI_Triggers.Trigger[i].y2)
						if(GUI_Triggers.Trigger[i].doPress != 0)
						{
							onPress = (uint32_t (*)(uint8_t))GUI_Triggers.Trigger[i].doPress;
							onPress(i);
							GUI_EX = 1;
						}
			GUI_FT = 1;
			GUI_CT = 0;
		}

		if(GUI_Pointed != 255 && GUI_EX == 0)
			if(GUI_Triggers.Trigger[GUI_Pointed].doOver != 0)
			{
				onOver = (uint32_t (*)(uint8_t))GUI_Triggers.Trigger[GUI_Pointed].doOver;
				onOver(GUI_Pointed);
			}
		GUI_Pointed = 255;
		GUI_CT = 0;
		GUI_EX = 0;
	}
}

int8_t GUI_CreateButton(uint32_t onselect, uint32_t onpoint, uint32_t onover, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint8_t i;
	TButton button;
	button.x1 = x1;
	button.y1 = y1;
	button.x2 = x2;
	button.y2 = y2;
	button.doPress = onselect;
	button.doOver = onover;
	button.doPoint = onpoint;

	for(i=0;i<TRIGGERS;i++)
	{
		if(GUI_Triggers.enabled[i] == 0)
		{
			GUI_Triggers.enabled[i] = 1;
			GUI_Triggers.Trigger[i] = button;
			return i;
		}
	}
	return -1;
}

