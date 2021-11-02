#include "stm32f4xx_hal.h"
#include "delay.h"

#define Y_SIZE 240
#define X_SIZE 320


#define graph_command(reg,value) { *(volatile uint16_t *)((uint32_t)0x60000000) = reg; *(volatile uint16_t *)((uint32_t)0x60020000) = value; }
#define graph_data(value) {*(volatile uint16_t *)((uint32_t)0x60020000)=value;}

#define LCD_IO_DAT(V) {*(volatile uint16_t *)((uint32_t)0x60020000) = V;}
#define LCD_IO_XY(Y,X) {graph_command(33,(X));graph_command(32,(239-(Y)));*(volatile uint16_t *)((uint32_t)0x60000000) = 34;}
#define LCD_GRAM(y,x,v) {LCD_IO_XY(y,x);LCD_IO_DAT(v);}

void graph_init(void);
