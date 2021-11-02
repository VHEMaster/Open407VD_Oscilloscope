#include "stm32f4xx_hal.h"
#include "ili9325.h"
#include "xpt2046.h"
#include "graph.h"



void graph_init(void)
{

    DelayMs(50);

    //************* Start Initial Sequence **********//
    graph_command(0x00E3, 0x3008); // Set internal timing
    graph_command(0x00E7, 0x0012); // Set internal timing
    graph_command(0x00EF, 0x1231); // Set internal timing
    graph_command(0x0001, 0x0100); // set SS and SM bit
    graph_command(0x0002, 0x0700); // set 1 line inversion
    graph_command(0x0003, 0x1028); // ÃÎÐ ÎÐÈÅÍ, > 
    graph_command(0x0004, 0x0000); // Resize register
    graph_command(0x0008, 0x0207); // set the back porch and front porch
    graph_command(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
    graph_command(0x000A, 0x0000); // FMARK function
    graph_command(0x000C, 0x0000); // RGB interface setting
    graph_command(0x000D, 0x0000); // Frame marker Position
    graph_command(0x000F, 0x0000); // RGB interface polarity
    //*************Power On sequence ****************//
     graph_command(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
     graph_command(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
     graph_command(0x0012, 0x0000); // VREG1OUT voltage
     graph_command(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
     DelayMs(200); // Dis-charge capacitor power voltage
     graph_command(0x0010, 0x1490); // SAP, BT[3:0], AP, DSTB, SLP, STB
     graph_command(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
     DelayMs(50); // Delay 50ms
     graph_command(0x0012, 0x001C); // Internal reference voltage= Vci;
     DelayMs(50); // Delay 50ms
     graph_command(0x0013, 0x1A00); // Set VDV[4:0] for VCOM amplitude
     graph_command(0x0029, 0x0025); // Set VCM[5:0] for VCOMH
     graph_command(0x002B, 0x000E); // Set Frame Rate
     DelayMs(50); // Delay 50ms
     graph_command(0x0020, 0x0000); // GRAM horizontal Address
     graph_command(0x0021, 0x0000); // GRAM Vertical Address
     // ----------- Adjust the Gamma Curve ----------//
     graph_command(0x0030, 0x0000);
     graph_command(0x0031, 0x0506);
     graph_command(0x0032, 0x0104);
     graph_command(0x0035, 0x0207);
     graph_command(0x0036, 0x000F);
     graph_command(0x0037, 0x0306);
     graph_command(0x0038, 0x0102);
     graph_command(0x0039, 0x0707);
     graph_command(0x003C, 0x0702);
     graph_command(0x003D, 0x1604);
     //------------------ Set GRAM area ---------------//
     graph_command(0x0050, 0x0000); // Horizontal GRAM Start Address
     graph_command(0x0051, 0x00EF); // Horizontal GRAM End Address
     graph_command(0x0052, 0x0000); // Vertical GRAM Start Address
     graph_command(0x0053, 0x013F); // Vertical GRAM Start Address
     graph_command(0x0060, 0xA700); // Gate Scan Line
     graph_command(0x0061, 0x0001); // NDL,VLE, REV
     graph_command(0x006A, 0x0000); // set scrolling line
     //-------------- Partial Display Control ---------//
     graph_command(0x0080, 0x0000);
     graph_command(0x0081, 0x0000);
     graph_command(0x0082, 0x0000);
     graph_command(0x0083, 0x0000);
     graph_command(0x0084, 0x0000);
     graph_command(0x0085, 0x0000);
     //-------------- Panel Control -------------------//
     graph_command(0x0090, 0x0010);
     graph_command(0x0092, 0x0600);

     graph_command(0x0007, 0x0133); // 262K color and display ON


     DelayMs(50); // Delay 50ms
		  graph_command(33, 319);
		  graph_command(32, 0);
			
     uint32_t i,j;
		 
     *(volatile uint16_t *)((uint32_t)0x60000000) = 34;
	  for(i=0;i<240;i++)
		  for(j=0;j<320;j++)
			  *(volatile uint16_t *)((uint32_t)0x60020000) = 0;

     //for (i = 0; i < 153280; i++)
     //	 graph_data(0);




       //graph_start();
}
