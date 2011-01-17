#include "nokialcd_io.h"
#include "nokialcd_driver.h"
#include "nokialcd_controlcodes.h"

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
	
#ifdef EPSON
void LCDInitController() {
    LCDCommand(EP_DISCTL);  	// display control(EPSON)
    LCDData(0x0C);   		// 12 = 1100 - CL dividing ratio [don't divide] switching period 8H (default)
	LCDData(0x20);			// 130 line display
	LCDData(0x00);			// no inversely highlighted lines
	LCDData(0x01);			// dispersion
	
    LCDCommand(EP_COMSCN);  	// common scanning direction(EPSON)
    LCDData(0x01);
    
    LCDCommand(EP_OSCON);  	// internal oscillator ON(EPSON)
	
    LCDCommand(EP_SLPOUT);  	// sleep out(EPSON)

    LCDCommand(EP_PWRCTR); 	// power ctrl(EPSON)
    LCDData(0x0F);    		//everything on, no external reference resistors

	LCDCommand(EP_DISINV);  	// invert display mode(EPSON)

    LCDCommand(EP_DATCTL);  	// data control(EPSON)
    LCDData(0x00);			// normal column addressing
	LCDData(0x00);   		// normal RGB arrangement
	LCDData(0x02);			// 16-bit Grayscale Type A
	
    LCDCommand(EP_VOLCTR);  	// electronic volume, this is the contrast/brightness(EPSON)
	LCDData(0x28);   		// volume (contrast) setting - fine tuning, original
    LCDData(0x03);   		// internal resistor ratio - coarse adjustment

    LCDCommand(EP_NOP);  	// nop(EPSON)

	for(long j=0; j<300000; j++) asm volatile ("nop");

    LCDCommand(EP_DISON);   	// display on(EPSON)
}
#endif

#ifdef PHILLIPS
void LCDInitController() {
	LCDCommand(PH_SLEEPOUT);	//sleep out(PHILLIPS)
    
    LCDCommand(PH_BSTRON);		//Booset On(PHILLIPS)
	
	LCDCommand(PH_INVON);		// invert display mode(PHILLIPS)
    
	LCDCommand(PH_MADCTL);		//Memory Access Control(PHILLIPS)
	LCDData(0xC8);
	
	LCDCommand(PH_COLMOD);		//Set Color Mode(PHILLIPS)
	LCDData(0x02);	
	
	LCDCommand(PH_SETCON);		//Set Contrast(PHILLIPS)
	LCDData(0x30);	
	   
	LCDCommand(PH_NOPP);		// nop(PHILLIPS)

	for(long j=0; j<300000; j++) asm volatile ("nop");

	LCDCommand(PH_DISPON);	// display on(PHILLIPS)
}
#endif

static inline void LCDSetWindow(int x1, int y1, int x2, int y2) {
	LCDCommand(LCD_PASET);   // page start/end ram
	LCDData(y1 + LCD_Y_OFFSET);
	LCDData(y2 + LCD_Y_OFFSET);

	LCDCommand(LCD_CASET);   // column start/end ram
	LCDData(x1);
	LCDData(x2);
}

static inline void LCDBeginWrite() {
	LCDCommand(LCD_RAMWR);
}

void LCDWriteWindow(int x1, int y1, int x2, int y2) {
	LCDSetWindow(x1, y1, x2, y2);
	LCDBeginWrite();
}

inline void LCDWritePair(int c1, int c2) {
	LCDData(c1 >> 4);
	LCDData((c1<<4) | ((c2>>8) & 0x0f));
	LCDData(c2);
}

void LCDSetPixel(int color, int x, int y) {
	LCDWriteWindow(x, y, x, y);
	LCDWritePair(0, color);
}

void LCDFillBox(int color, int x1, int y1, int x2, int y2) {
	uint8_t c1 = color >> 4;
	uint8_t c2 = (color<<4) | ((color>>8) & 0x0f);
	uint8_t c3 = color;
	int num_words = ((x2-x1+1) * (y2-y1+1) + 1) / 2;

	LCDWriteWindow(x1, y1, x2, y2);
	for(int i=0; i<num_words; i++) {
		LCDData(c1);
		LCDData(c2);
		LCDData(c3);
	}
}

void LCDClearScreen(int color) {
	LCDFillBox(color, 0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
}
