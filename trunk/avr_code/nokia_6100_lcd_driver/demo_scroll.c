#include "nokialcd_driver.h"
#include "nokialcd_scroll.h"
#include "nokialcd_font.h"
#include <stdlib.h>

void sysInit();

const char *hexchars = "0123456789ABCDEF";

void drawHex8(int n, int x, int y) {
	char hex[3];
	hex[0] = hexchars[(n>>4) & 0xf];
	hex[1] = hexchars[n & 0xf];
	hex[2] = 0;
	drawString(hex, 0xfff, 0x000, x, y);
}

int main() {
	sysInit();
	LCDInitController();

	LCDClearScreen(0);

	drawString("[[ Top Area ]]", 0xf00, 0, 0, 0);
	drawString("[[ Bottom Area ]]", 0xf00, 0, 0, FONT_HEIGHT*15);
	char *text = "All work and no play make Jack a dull boy. ";

	int topsize = FONT_HEIGHT;
	int scrollsize = 14*FONT_HEIGHT;

	// NOTE: all parameters passed to scroll functions
	// must be divisible by four!
	// And in fact in the case of this demo the scrollsize
	// must be divisible by 8 so that a line of text will
	// not cross the loop boundary.
	scrollInit(topsize, scrollsize);

	int iter = 0;
	int tp = 0;
	for(;;) { 
		// scroll upward by 8 pixels
		scrollShift(8);

		// convert logical Y value to physical Y value
		int y = scrollConvertY(scrollsize-FONT_HEIGHT);

		drawHex8(iter++, 0, y);
		for(int i=3; i<LCD_WIDTH/FONT_WIDTH; i++) {
			drawChar(' ', 0x0f0, 0, i*FONT_WIDTH, y);
		}
		for(int i=3; i<LCD_WIDTH/FONT_WIDTH; i++) {
			long delay = rand();
			delay = delay*delay/5000;
			for(long j=0; j<delay; j++) asm volatile ("nop");
			drawChar(text[tp], 0x0f0, 0, i*FONT_WIDTH, y);
			tp++;
			if(!text[tp]) tp = 0;
		}
	}
}
