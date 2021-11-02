#include "stm32f4xx.h"

#define LongToBin(n) \
(\
((n >> 21) & 0x80) | \
((n >> 18) & 0x40) | \
((n >> 15) & 0x20) | \
((n >> 12) & 0x10) | \
((n >> 9) & 0x08) | \
((n >> 6) & 0x04) | \
((n >> 3) & 0x02) | \
((n ) & 0x01) \
)
#define BIN(n) LongToBin(0x##n##l)

uint8_t font_decode(uint8_t ansii)
{
	if(ansii >= 0xC0 && ansii <= 0xFF)
	{
		switch(ansii)
		{
			case 0xC0 : return 0x41;
			case 0xC1 : return 0xA0;
			case 0xB3 : return 0x69;
			case 0xC2 : return 0x42;
			case 0xC3 : return 0xA1;
			case 0xC4 : return 0xE0;
			case 0xC5 : return 0x45;
			case 0xC6 : return 0xA3;
			case 0xC7 : return 0xA4;
			case 0xC8 : return 0xA5;
			case 0xC9 : return 0xA6;
			case 0xCA : return 0x4B;
			case 0xCB : return 0xA7;
			case 0xCC : return 0x4D;
			case 0xCD : return 0x48;
			case 0xCE : return 0x4F;
			case 0xCF : return 0xA8;
			case 0xD0 : return 0x50;
			case 0xD1 : return 0x43;
			case 0xD2 : return 0x54;
			case 0xD3 : return 0xA9;
			case 0xD4 : return 0xAA;
			case 0xD5 : return 0x58;
			case 0xD6 : return 0xE1;
			case 0xD7 : return 0xAB;
			case 0xD8 : return 0xAC;
			case 0xD9 : return 0xE2;
			case 0xDA : return 0xAD;
			case 0xDB : return 0xAE;
			case 0xDC : return 0xC4;
			case 0xDD : return 0xAF;
			case 0xDE : return 0xB0;
			case 0xDF : return 0xB1;
			case 0xE0 : return 0x61;
			case 0xE1 : return 0xB2;
			case 0xE2 : return 0xB3;
			case 0xE3 : return 0xB4;
			case 0xE4 : return 0xE3;
			case 0xE5 : return 0x65;
			case 0xE6 : return 0xB6;
			case 0xE7 : return 0xB7;
			case 0xE9 : return 0xB9;
			case 0xEA : return 0xBA;
			case 0xEB : return 0xBB;
			case 0xEC : return 0xBC;
			case 0xED : return 0xBD;
			case 0xEE : return 0x6F;
			case 0xEF : return 0xBE;
			case 0xF0 : return 0x70;
			case 0xF1 : return 0x63;
			case 0xF2 : return 0xBF;
			case 0xF3 : return 0x79;
			case 0xF4 : return 0xE4;
			case 0xF5 : return 0x78;
			case 0xF6 : return 0xE5;
			case 0xF7 : return 0xC0;
			case 0xF8 : return 0xC1;
			case 0xF9 : return 0xE6;
			case 0xFA : return 0xC2;
			case 0xFB : return 0xC3;
			case 0xFC : return 0xC4;
			case 0xFD : return 0xC5;
			case 0xFE : return 0xC6;
			case 0xFF : return 0xC7;
		}
	}
	return ansii;
}


uint8_t font_get(uint8_t addr, uint8_t n)
{
	uint8_t fnt[7];
	uint8_t naddr = font_decode(addr);
	switch(naddr)
	{
		case 0x00 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00000000);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00000000);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00000000);
			break;
		case 0x30 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010011);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00011001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case '%' :
			fnt[0] = BIN(00011000);
			fnt[1] = BIN(00011001);
			fnt[2] = BIN(00000010);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00010011);
			fnt[6] = BIN(00000011);
			break;
		case '/' :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000001);
			fnt[2] = BIN(00000010);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00000000);
			break;
		case '\\' :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00001000);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00000010);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00000000);
			break;
		case 0x31 :
			fnt[0] = BIN(00000100);
			fnt[1] = BIN(00001100);
			fnt[2] = BIN(00000100);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00001110);
			break;
		case 0x32 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00000001);
			fnt[3] = BIN(00000010);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00001000);
			fnt[6] = BIN(00011111);
			break;
		case 0x33 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00000010);
			fnt[2] = BIN(00000100);
			fnt[3] = BIN(00000010);
			fnt[4] = BIN(00000001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x34 :
			fnt[0] = BIN(00000010);
			fnt[1] = BIN(00000110);
			fnt[2] = BIN(00001010);
			fnt[3] = BIN(00010010);
			fnt[4] = BIN(00011111);
			fnt[5] = BIN(00000010);
			fnt[6] = BIN(00000010);
			break;
		case 0x35 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00011110);
			fnt[3] = BIN(00000001);
			fnt[4] = BIN(00000001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x36 :
			fnt[0] = BIN(00000110);
			fnt[1] = BIN(00001000);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00011110);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x37 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00000001);
			fnt[2] = BIN(00000010);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00001000);
			fnt[6] = BIN(00001000);
			break;
		case 0x38 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00001110);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x39 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00001111);
			fnt[4] = BIN(00000001);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00001110);
			break;
		case 0x20 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00000000);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00000000);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00000000);
			break;
		case 0x41 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00011111);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0x42 :
			fnt[0] = BIN(00011110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00011110);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00011110);
			break;
		case 0x43 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00010000);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x44 :
			fnt[0] = BIN(00011100);
			fnt[1] = BIN(00010010);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010010);
			fnt[6] = BIN(00011100);
			break;
		case 0x45 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00011111);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00011111);
			break;
		case 0x46 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00011110);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00010000);
			break;
		case 0x47 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00010111);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001111);
			break;
		case 0x48 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00011111);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0x49 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00000100);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00001110);
			break;
		case 0x4A :
			fnt[0] = BIN(00000111);
			fnt[1] = BIN(00000010);
			fnt[2] = BIN(00000010);
			fnt[3] = BIN(00000010);
			fnt[4] = BIN(00000010);
			fnt[5] = BIN(00010010);
			fnt[6] = BIN(00001100);
			break;
		case 0x4B :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010010);
			fnt[2] = BIN(00010100);
			fnt[3] = BIN(00011000);
			fnt[4] = BIN(00010100);
			fnt[5] = BIN(00010010);
			fnt[6] = BIN(00010001);
			break;
		case 0x4C :
			fnt[0] = BIN(00010000);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00010000);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00011111);
			break;
		case 0x4D :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00011011);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0x4E :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00011001);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010011);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0x4F :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x50 :
			fnt[0] = BIN(00011110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00011110);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00010000);
			break;
		case 0x51 :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010010);
			fnt[6] = BIN(00001101);
			break;
		case 0x52 :
			fnt[0] = BIN(00011110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00011110);
			fnt[4] = BIN(00010100);
			fnt[5] = BIN(00010010);
			fnt[6] = BIN(00010001);
			break;
		case 0x53 :
			fnt[0] = BIN(00001111);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00001110);
			fnt[4] = BIN(00000001);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00011110);
			break;
		case 0x54 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00000100);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00000100);
			break;
		case 0x55 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x56 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00001010);
			fnt[6] = BIN(00000100);
			break;
		case 0x57 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00001010);
			break;
		case 0x58 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00001010);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00001010);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0x59 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00001010);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00000100);
			break;
		case 0x5A :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00000001);
			fnt[2] = BIN(00000010);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00011111);
			break;
		case 0x61 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00000001);
			fnt[4] = BIN(00001111);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001111);
			break;
		case 0x62 :
			fnt[0] = BIN(00010000);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00010110);
			fnt[3] = BIN(00011001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00011110);
			break;
		case 0x63 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00010000);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x64 :
			fnt[0] = BIN(00000001);
			fnt[1] = BIN(00000001);
			fnt[2] = BIN(00001101);
			fnt[3] = BIN(00010011);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001111);
			break;
		case 0x65 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00011111);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00001110);
			break;
		case 0xB8 :
			fnt[0] = BIN(00001010);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00011111);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00001110);
			break;
		case 0x66 :
			fnt[0] = BIN(00000110);
			fnt[1] = BIN(00001001);
			fnt[2] = BIN(00001000);
			fnt[3] = BIN(00011100);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00001000);
			fnt[6] = BIN(00001000);
			break;
		case 0x67 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00001111);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00001111);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00001110);
			break;
		case 0x68 :
			fnt[0] = BIN(00010000);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00010110);
			fnt[3] = BIN(00011001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0x69 :
			fnt[0] = BIN(00000100);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001100);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00001110);
			break;
		case 0x6A :
			fnt[0] = BIN(00000010);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00000110);
			fnt[3] = BIN(00000010);
			fnt[4] = BIN(00000010);
			fnt[5] = BIN(00010010);
			fnt[6] = BIN(00001100);
			break;
		case 0x6B :
			fnt[0] = BIN(00010000);
			fnt[1] = BIN(00010000);
			fnt[2] = BIN(00010010);
			fnt[3] = BIN(00010100);
			fnt[4] = BIN(00011000);
			fnt[5] = BIN(00010100);
			fnt[6] = BIN(00010010);
			break;
		case 0x6C :
			fnt[0] = BIN(00001100);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00000100);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00001110);
			break;
		case 0x6D :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011010);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0x6E :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010110);
			fnt[3] = BIN(00011001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0x6F :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0x70 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011110);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00011110);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00010000);
			break;
		case 0x71 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001101);
			fnt[3] = BIN(00010011);
			fnt[4] = BIN(00001111);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00000001);
			break;
		case 0x72 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010110);
			fnt[3] = BIN(00011001);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00010000);
			break;
		case 0x73 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00010000);
			fnt[4] = BIN(00001110);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00001110);
			break;
		case 0x74 :
			fnt[0] = BIN(00001000);
			fnt[1] = BIN(00001000);
			fnt[2] = BIN(00011100);
			fnt[3] = BIN(00001000);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00001001);
			fnt[6] = BIN(00000110);
			break;
		case 0x75 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010011);
			fnt[6] = BIN(00001101);
			break;
		case 0x76 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00001010);
			fnt[6] = BIN(00000100);
			break;
		case 0x77 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00001010);
			break;
		case 0x78 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00001010);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00001010);
			fnt[6] = BIN(00010001);
			break;
		case 0x79 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00001111);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00001110);
			break;
		case 0x7A :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011111);
			fnt[3] = BIN(00000010);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00001000);
			fnt[6] = BIN(00011111);
			break;
		case 0xA0 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00011110);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00011110);
			break;
		case 0xA1 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00010000);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00010000);
			break;
		case 0xA2 :
			fnt[0] = BIN(00001010);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011110);
			fnt[3] = BIN(00010000);
			fnt[4] = BIN(00011100);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00011110);
			break;
		case 0xA3 :
			fnt[0] = BIN(00010101);
			fnt[1] = BIN(00010101);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00001110);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00010101);
			break;
		case 0xA4 :
			fnt[0] = BIN(00011110);
			fnt[1] = BIN(00000001);
			fnt[2] = BIN(00000001);
			fnt[3] = BIN(00001111);
			fnt[4] = BIN(00000001);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00011110);
			break;
		case 0xA5 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010011);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00011001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0xA6 :
			fnt[0] = BIN(00001010);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010011);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00011001);
			fnt[6] = BIN(00010001);
			break;
		case 0xA7 :
			fnt[0] = BIN(00001111);
			fnt[1] = BIN(00000101);
			fnt[2] = BIN(00000101);
			fnt[3] = BIN(00000101);
			fnt[4] = BIN(00000101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00001001);
			break;
		case 0xA8 :
			fnt[0] = BIN(00011111);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0xA9 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00001010);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00001000);
			fnt[6] = BIN(00010000);
			break;
		case 0xAA :
			fnt[0] = BIN(00000100);
			fnt[1] = BIN(00001110);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00001110);
			fnt[6] = BIN(00000100);
			break;
		case 0xAB :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00001111);
			fnt[4] = BIN(00000001);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00000001);
			break;
		case 0xAC :
			fnt[0] = BIN(00010101);
			fnt[1] = BIN(00010101);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00011111);
			break;
		case 0xAD :
			fnt[0] = BIN(00011000);
			fnt[1] = BIN(00001000);
			fnt[2] = BIN(00001000);
			fnt[3] = BIN(00001110);
			fnt[4] = BIN(00001001);
			fnt[5] = BIN(00001001);
			fnt[6] = BIN(00001110);
			break;
		case 0xAE :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00011001);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00011001);
			break;
		case 0xAF :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00000001);
			fnt[3] = BIN(00000111);
			fnt[4] = BIN(00000001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0xB0 :
			fnt[0] = BIN(00010010);
			fnt[1] = BIN(00010101);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00011101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00010010);
			break;
		case 0xB1 :
			fnt[0] = BIN(00001111);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00001111);
			fnt[4] = BIN(00000101);
			fnt[5] = BIN(00001001);
			fnt[6] = BIN(00010001);
			break;
		case 0xB2 :
			fnt[0] = BIN(00000011);
			fnt[1] = BIN(00001100);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00011110);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0xB3 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011100);
			fnt[3] = BIN(00010010);
			fnt[4] = BIN(00011100);
			fnt[5] = BIN(00010010);
			fnt[6] = BIN(00011100);
			break;
		case 0xB4 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011111);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010000);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00010000);
			break;
		case 0xB5 :
			fnt[0] = BIN(00001010);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00011111);
			fnt[5] = BIN(00010000);
			fnt[6] = BIN(00001110);
			break;
		case 0xB6 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00001110);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00010101);
			break;
		case 0xB7 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011110);
			fnt[3] = BIN(00000001);
			fnt[4] = BIN(00000110);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00011110);
			break;
		case 0xE8 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010011);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00011001);
			fnt[6] = BIN(00010001);
			break;
		case 0xB9 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00001010);
			fnt[2] = BIN(00000100);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010011);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00011001);
			break;
		case 0xBA :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010010);
			fnt[3] = BIN(00010100);
			fnt[4] = BIN(00011000);
			fnt[5] = BIN(00010100);
			fnt[6] = BIN(00010010);
			break;
		case 0xBB :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001111);
			fnt[3] = BIN(00000101);
			fnt[4] = BIN(00000101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00001001);
			break;
		case 0xBC :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00011011);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0xBD :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00011111);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0xBE :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011111);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00010001);
			break;
		case 0xBF :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011111);
			fnt[3] = BIN(00000100);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00000100);
			break;
		case 0xC0 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00001111);
			fnt[5] = BIN(00000001);
			fnt[6] = BIN(00000001);
			break;
		case 0xC1 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00011111);
			break;
		case 0xC2 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011000);
			fnt[3] = BIN(00001000);
			fnt[4] = BIN(00001110);
			fnt[5] = BIN(00001001);
			fnt[6] = BIN(00001110);
			break;
		case 0xC3 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00011001);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00011001);
			break;
		case 0xC4 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010000);
			fnt[3] = BIN(00010000);
			fnt[4] = BIN(00011100);
			fnt[5] = BIN(00010010);
			fnt[6] = BIN(00011100);
			break;
		case 0xC5 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00000111);
			fnt[5] = BIN(00010001);
			fnt[6] = BIN(00001110);
			break;
		case 0xC6 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010010);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00011101);
			fnt[5] = BIN(00010101);
			fnt[6] = BIN(00010010);
			break;
		case 0xC7 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00001111);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00001111);
			fnt[5] = BIN(00000101);
			fnt[6] = BIN(00001001);
			break;
		case 0xE0 :
			fnt[0] = BIN(00001111);
			fnt[1] = BIN(00000101);
			fnt[2] = BIN(00000101);
			fnt[3] = BIN(00001001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00011111);
			fnt[6] = BIN(00010001);
			break;
		case 0xE1 :
			fnt[0] = BIN(00010001);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00011111);
			fnt[6] = BIN(00000001);
			break;
		case 0xE2 :
			fnt[0] = BIN(00010101);
			fnt[1] = BIN(00010101);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00011111);
			fnt[6] = BIN(00000001);
			break;
		case 0xE3 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00011111);
			fnt[2] = BIN(00000101);
			fnt[3] = BIN(00001001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00011111);
			fnt[6] = BIN(00010001);
			break;
		case 0xE4 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00001110);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00001110);
			fnt[6] = BIN(00000100);
			break;
		case 0xE5 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00010001);
			fnt[4] = BIN(00010001);
			fnt[5] = BIN(00011111);
			fnt[6] = BIN(00000001);
			break;
		case 0xE6 :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00010101);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00011111);
			fnt[6] = BIN(00000001);
			break;
		case 0x21 :
			fnt[0] = BIN(00001000);
			fnt[1] = BIN(00001000);
			fnt[2] = BIN(00001000);
			fnt[3] = BIN(00001000);
			fnt[4] = BIN(00000000);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00001000);
			break;
		case 0x27 :
			fnt[0] = BIN(00001100);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00001000);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00000000);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00000000);
			break;
		case 0x28 :
			fnt[0] = BIN(00000010);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00001000);
			fnt[3] = BIN(00001000);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00000010);
			break;
		case 0x29 :
			fnt[0] = BIN(00001000);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00000010);
			fnt[3] = BIN(00000010);
			fnt[4] = BIN(00000010);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00001000);
			break;
		case 0x2A :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00010101);
			fnt[3] = BIN(00001110);
			fnt[4] = BIN(00010101);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00000000);
			break;
		case 0x2B :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00000100);
			fnt[3] = BIN(00011111);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00000000);
			break;
		case 0x2C :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00000000);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00001100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00001000);
			break;
		case 0x2D :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00000000);
			fnt[3] = BIN(00011111);
			fnt[4] = BIN(00000000);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00000000);
			break;
		case 0x2E :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00000000);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00000000);
			fnt[5] = BIN(00001100);
			fnt[6] = BIN(00001100);
			break;
		case 0x3A :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00001100);
			fnt[2] = BIN(00001100);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00001100);
			fnt[5] = BIN(00001100);
			fnt[6] = BIN(00000000);
			break;
		case 0x3B :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00001100);
			fnt[2] = BIN(00001100);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00001100);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00001000);
			break;
		case 0x3C :
			fnt[0] = BIN(00000010);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00001000);
			fnt[3] = BIN(00010000);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00000010);
			break;
		case 0x3D :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00011111);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00011111);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00000000);
			break;
		case 0x3E :
			fnt[0] = BIN(00001000);
			fnt[1] = BIN(00000100);
			fnt[2] = BIN(00000010);
			fnt[3] = BIN(00000001);
			fnt[4] = BIN(00000010);
			fnt[5] = BIN(00000100);
			fnt[6] = BIN(00001000);
			break;
		case 0x3F :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00010001);
			fnt[2] = BIN(00000001);
			fnt[3] = BIN(00000010);
			fnt[4] = BIN(00000100);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00000100);
			break;
		case 0x5E :
			fnt[0] = BIN(00000100);
			fnt[1] = BIN(00001010);
			fnt[2] = BIN(00010001);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00000000);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00000000);
			break;
		case 0x5F :
			fnt[0] = BIN(00000000);
			fnt[1] = BIN(00000000);
			fnt[2] = BIN(00000000);
			fnt[3] = BIN(00000000);
			fnt[4] = BIN(00000000);
			fnt[5] = BIN(00000000);
			fnt[6] = BIN(00011111);
			break;
		case 0x5B :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00001000);
			fnt[2] = BIN(00001000);
			fnt[3] = BIN(00001000);
			fnt[4] = BIN(00001000);
			fnt[5] = BIN(00001000);
			fnt[6] = BIN(00001110);
			break;
		case 0x5D :
			fnt[0] = BIN(00001110);
			fnt[1] = BIN(00000010);
			fnt[2] = BIN(00000010);
			fnt[3] = BIN(00000010);
			fnt[4] = BIN(00000010);
			fnt[5] = BIN(00000010);
			fnt[6] = BIN(00001110);
			break;
		default : return 0;
	}
	switch(n)
	{
		case 0 : return fnt[0];
		case 1 : return fnt[1];
		case 2 : return fnt[2];
		case 3 : return fnt[3];
		case 4 : return fnt[4];
		case 5 : return fnt[5];
		case 6 : return fnt[6];
	}
	return 0;
}
