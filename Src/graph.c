#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "stm32f4xx.h"
#include "font.h"
#include "stdlib.h"
#include "ili9325.h"
#include "graph.h"


void graph_clear(void)
{
	uint16_t i,j;
  LCD_IO_XY(0,0);
	for(i=0;i<240;i++)
		for(j=0;j<320;j++)
			LCD_IO_DAT(0);
}

void graph_print(uint16_t x, uint16_t y, uint16_t color, uint8_t fsize, char* _string, ...)
{
	uint16_t i,j,o;
	uint8_t t;
	char _str[128];
	va_list ap;
	va_start(ap, _string);
	vsprintf(_str, _string, ap);

	o = 0;
	if(fsize == 1)
	{
		while(_str[o])
		{
			  for(i=0;i<7;i++)
				{
					t = font_get(_str[o],i);
				  for(j=0;j<5;j++)
					{
					  if((t>>(4-j))&1)
						  LCD_GRAM(i+y,j+x+(o*6),color);
					}
				}
			  o++;
		}
	}
	else if(fsize == 2)
	{
		while(_str[o])
		{
			  for(i=0;i<7;i++)
				{
					t = font_get(_str[o],i);
				  for(j=0;j<5;j++)
					  if((t>>(4-j))&1)
					  {
						  LCD_GRAM((i*2)+y+1,(j*2)+x+(o*12),color);
						  LCD_GRAM((i*2)+y,(j*2)+x+(o*12),color);
						  LCD_GRAM((i*2)+y+1,(j*2)+x+(o*12)+1,color);
						  LCD_GRAM((i*2)+y,(j*2)+x+(o*12)+1,color);
					  }
				}
			  o++;
		}
	}
	else if(fsize == 3)
	{
		while(_str[o])
		{
			  for(i=0;i<7;i++)
				{
					t = font_get(_str[o],i);
				  for(j=0;j<5;j++)
					  if((t>>(4-j))&1)
						{
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18),color);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18),color);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18),color);
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18)+1,color);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18)+1,color);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18)+1,color);
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18)+2,color);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18)+2,color);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18)+2,color);
					  }
				}
			  o++;
		}
	}


	va_end(ap);
}

void graph_print_bg(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t fsize, char* _string, ...)
{
	uint16_t i,j,o;
	uint8_t t;
	char _str[128];
	va_list ap; 
	va_start(ap, _string);
	vsprintf(_str, _string, ap);

	o = 0;
	if(fsize == 1)
	{
		while(_str[o])
		{
			  for(i=0;i<7;i++)
			  {
					t = font_get(_str[o],i);
				  for(j=0;j<5;j++)
					  if((t>>(4-j))&1)
						  { LCD_GRAM((i+y),(j+x+(o*6)),color); }
					  else LCD_GRAM(i+y,j+x+(o*6),bgcolor);
				  LCD_GRAM(i+y,x+(o*6)+5,bgcolor);
			  }
			  o++;
		}
	}
	else if(fsize == 2)
	{
		while(_str[o])
		{
			  for(i=0;i<7;i++)
			  {
					t = font_get(_str[o],i);
				  for(j=0;j<5;j++)
					  if((t>>(4-j))&1)
					  {
						  LCD_GRAM((i*2)+y+1,(j*2)+x+(o*12),color);
						  LCD_GRAM((i*2)+y,(j*2)+x+(o*12),color);
						  LCD_GRAM((i*2)+y+1,(j*2)+x+(o*12)+1,color);
						  LCD_GRAM((i*2)+y,(j*2)+x+(o*12)+1,color);
					  }
					  else
					  {
						  LCD_GRAM((i*2)+y+1,(j*2)+x+(o*12),bgcolor);
						  LCD_GRAM((i*2)+y,(j*2)+x+(o*12),bgcolor);
						  LCD_GRAM((i*2)+y+1,(j*2)+x+(o*12)+1,bgcolor);
						  LCD_GRAM((i*2)+y,(j*2)+x+(o*12)+1,bgcolor);
					  }
				  LCD_GRAM((i*2)+y,x+(o*12)+10,bgcolor);
				  LCD_GRAM((i*2)+y+1,x+(o*12)+10,bgcolor);
				  LCD_GRAM((i*2)+y,x+(o*12)+11,bgcolor);
				  LCD_GRAM((i*2)+y+1,x+(o*12)+11,bgcolor);
			  }
			  o++;
		}
	}
	else if(fsize == 3)
	{
		while(_str[o])
		{
			  for(i=0;i<7;i++)
			  {
					t = font_get(_str[o],i);
				  for(j=0;j<5;j++)
					  if((t>>(4-j))&1)
					  {
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18),color);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18),color);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18),color);
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18)+1,color);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18)+1,color);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18)+1,color);
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18)+2,color);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18)+2,color);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18)+2,color);
					  }
					  else
					  {
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18),bgcolor);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18),bgcolor);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18),bgcolor);
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18)+1,bgcolor);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18)+1,bgcolor);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18)+1,bgcolor);
						  LCD_GRAM((i*3)+y+1,(j*3)+x+(o*18)+2,bgcolor);
						  LCD_GRAM((i*3)+y+2,(j*3)+x+(o*18)+2,bgcolor);
						  LCD_GRAM((i*3)+y,(j*3)+x+(o*18)+2,bgcolor);
					  }
				  LCD_GRAM((i*3)+y,x+(o*18)+15,bgcolor);
				  LCD_GRAM((i*3)+y+1,x+(o*18)+15,bgcolor);
				  LCD_GRAM((i*3)+y+2,x+(o*18)+15,bgcolor);
				  LCD_GRAM((i*3)+y,x+(o*18)+16,bgcolor);
				  LCD_GRAM((i*3)+y+1,x+(o*18)+16,bgcolor);
				  LCD_GRAM((i*3)+y+2,x+(o*18)+16,bgcolor);
				  LCD_GRAM((i*3)+y,x+(o*18)+17,bgcolor);
				  LCD_GRAM((i*3)+y+1,x+(o*18)+17,bgcolor);
				  LCD_GRAM((i*3)+y+2,x+(o*18)+17,bgcolor);
			  }

			  o++;
		}
	}


	va_end(ap);
}

void graph_print_string(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t fsize, uint8_t ssize, char* _string, ...)
{
	uint8_t i;
	char _str[128];
	va_list ap;
	va_start(ap, _string);
	vsprintf(_str, _string, ap);
	uint8_t strl = strlen((char *)_str);
	if(strl>ssize) ssize = strl;
	for(i=strl;i<ssize;i++)
		_str[i] = 0x20;
	_str[ssize] = 0;
	graph_print_bg(x, y, color, bgcolor, fsize, _str);
}

void graph_square(uint16_t line_color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t i;
	for(i=y1;i<y2;i++)
	{
		LCD_GRAM(i,x1,line_color);
		LCD_GRAM(i,x2,line_color);
	}
	for(i=x1;i<=x2;i++)
	{
		LCD_GRAM(y1,i,line_color);
		LCD_GRAM(y2,i,line_color);
	}

}

void graph_square_bg(uint16_t line_color, uint16_t bg_color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t i,j;
	for(i=x1;i<=x2;i++)
	{
		LCD_GRAM(y1,i,line_color);
		LCD_GRAM(y2,i,line_color);
	}
	for(i=y1;i<=y2;i++)
	{
		LCD_GRAM(i,x1,line_color);
		LCD_GRAM(i,x2,line_color);
	}

		for(j=y1+1;j<y2;j++)
			for(i=x1+1;i<x2;i++)
				LCD_GRAM(j,i,bg_color);
}



void graph_esquare(uint16_t line_color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t i,xx,xx2,yy2;
	xx2 = x2-3;
	yy2 = y2-3;
	xx = x2-x1;
	for(i=x1+3;i<=xx2;i++)
	{
		LCD_GRAM(y1,i,line_color);
		LCD_GRAM(y2,i,line_color);
	}
	for(i=y1+3;i<=yy2;i++)
	{
		LCD_GRAM(i,x1,line_color);
		LCD_GRAM(i,x2,line_color);
	}

	if(xx >= 4)
	{
		LCD_GRAM(y1+1,x1+2,line_color);
		LCD_GRAM(y2-1,x1+2,line_color);
		LCD_GRAM(y1+1,x2-2,line_color);
		LCD_GRAM(y2-1,x2-2,line_color);
	}
	if(xx >= 2)
	{
		LCD_GRAM(y2-2,x2-1,line_color);
		LCD_GRAM(y1+2,x1+1,line_color);
		LCD_GRAM(y2-2,x1+1,line_color);
		LCD_GRAM(y1+2,x2-1,line_color);
	}

}

void graph_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) 
{ 
	if(x1>=x0 && y1>=y0)
	{
		if(x1-x0 >= y1-y0)
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y0,x;
			for(x=x0;x<=x1;x++)
			{
					LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x0,y;
			for(y=y0;y<=y1;y++)
			{
					LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x0>=x1 && y0>=y1)
	{
		if(x0-x1 >= y0-y1)
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y1,x;
			for(x=x1;x<=x0;x++)
			{
					LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x1,y;
			for(y=y1;y<=y0;y++)
			{
					LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x0>=x1 && y1>=y0)
	{
		if(x0-x1 >= y1-y0)
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y0,x;
			for(x=x0;x>=x1;x--)
			{
					LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x1,y;
			for(y=y1;y>=y0;y--)
			{
					LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x1>=x0 && y0>=y1)
	{
		if(x1-x0 >= y0-y1)
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y1,x;
			for(x=x1;x>=x0;x--)
			{
					LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x0,y;
			for(y=y0;y>=y1;y--)
			{
					LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	
}


void graph_line_limited2(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lim_x0, int16_t lim_y0, int16_t lim_x1, int16_t lim_y1, int16_t lim_x2, int16_t lim_y2, int16_t lim_x3, int16_t lim_y3, uint16_t color)
{
	if(x1>=x0 && y1>=y0)
	{
		if(x1-x0 >= y1-y0)
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y0,x;
			for(x=x0;x<=x1;x++)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1)
						if(x < lim_x2 && x > lim_x3 && y < lim_y2 && y > lim_y3) 
							LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x0,y;
			for(y=y0;y<=y1;y++)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1)
						if(x < lim_x2 && x > lim_x3 && y < lim_y2 && y > lim_y3) 
							LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x0>=x1 && y0>=y1)
	{
		if(x0-x1 >= y0-y1)
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y1,x;
			for(x=x1;x<=x0;x++)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1)
						if(x < lim_x2 && x > lim_x3 && y < lim_y2 && y > lim_y3) 
							LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x1,y;
			for(y=y1;y<=y0;y++)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1)
						if(x < lim_x2 && x > lim_x3 && y < lim_y2 && y > lim_y3) 
							LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x0>=x1 && y1>=y0)
	{
		if(x0-x1 >= y1-y0)
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y0,x;
			for(x=x0;x>=x1;x--)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1)
						if(x < lim_x2 && x > lim_x3 && y < lim_y2 && y > lim_y3) 
							LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x1,y;
			for(y=y1;y>=y0;y--)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1)
						if(x < lim_x2 && x > lim_x3 && y < lim_y2 && y > lim_y3) 
							LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x1>=x0 && y0>=y1)
	{
		if(x1-x0 >= y0-y1)
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y1,x;
			for(x=x1;x>=x0;x--)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1)
						if(x < lim_x2 && x > lim_x3 && y < lim_y2 && y > lim_y3) 
							LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x0,y;
			for(y=y0;y>=y1;y--)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1)
						if(x < lim_x2 && x > lim_x3 && y < lim_y2 && y > lim_y3) 
							LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
}

void graph_line_limited(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t lim_x0, int16_t lim_y0, int16_t lim_x1, int16_t lim_y1, uint16_t color)
{ 
	if(x1>=x0 && y1>=y0)
	{
		if(x1-x0 >= y1-y0)
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y0,x;
			for(x=x0;x<=x1;x++)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1) LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x0,y;
			for(y=y0;y<=y1;y++)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1) LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x0>=x1 && y0>=y1)
	{
		if(x0-x1 >= y0-y1)
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y1,x;
			for(x=x1;x<=x0;x++)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1) LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x1,y;
			for(y=y1;y<=y0;y++)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1) LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x0>=x1 && y1>=y0)
	{
		if(x0-x1 >= y1-y0)
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y0,x;
			for(x=x0;x>=x1;x--)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1) LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x0 - x1);
			int16_t deltay = abs(y1 - y0);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x1,y;
			for(y=y1;y>=y0;y--)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1) LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	else if(x1>=x0 && y0>=y1)
	{
		if(x1-x0 >= y0-y1)
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltay;
			int16_t y = y1,x;
			for(x=x1;x>=x0;x--)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1) LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltax)
					{
							y = y + 1;
							error = error - deltax;	
					}
			}
		}
		else
		{
			int16_t deltax = abs(x1 - x0);
			int16_t deltay = abs(y0 - y1);
			int16_t error = 0;
			int16_t deltaerr = deltax;
			int16_t x = x0,y;
			for(y=y0;y>=y1;y--)
			{
					if(x >= lim_x0 && x <= lim_x1 && y >= lim_y0 && y <= lim_y1) LCD_GRAM(y,x,color);
					error = error+deltaerr;
					if(2*error >= deltay)
					{
							x = x + 1;
							error = error - deltay;	
					}
			}
		}
	}
	
}



