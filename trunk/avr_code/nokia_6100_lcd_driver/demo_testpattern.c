#include "nokialcd_driver.h"
#include "nokialcd_scroll.h"
#include "nokialcd_font.h"
#include <string.h>

void sysInit();

int main() {
	sysInit();
	LCDInitController();

	for(int color_idx=0; ; color_idx=(color_idx+1)%16) {
		int fg = 
			((color_idx & 1) ? 0x00f : 0) |
			((color_idx & 2) ? 0x0f0 : 0) |
			((color_idx & 4) ? 0xf00 : 0);
		int bg = 0;
		if(color_idx & 8) {
			bg = fg;
			fg = 0;
		}
		if(fg == bg) continue;

		LCDClearScreen(bg);

		// Draw dotted-line border around edge of display.
		// This is a way to check that LCD_WIDTH, LCD_HEIGHT, and
		// LCD_Y_OFFSET are set to proper values.
		for(int i=0; i<LCD_WIDTH; i++) {
			if(i & 1) {
				LCDSetPixel(fg, i, 0);
				LCDSetPixel(fg, i, LCD_HEIGHT-1);
			}
		}
		for(int i=0; i<LCD_HEIGHT; i++) {
			if(i & 1) {
				LCDSetPixel(fg, 0, i);
				LCDSetPixel(fg, LCD_WIDTH-1, i);
			}
		}

		// test box drawing for various size boxes
		int x, y;
		y = 2;
		for(int j=0; j<4; j++) {
			x = 2;
			for(int i=0; i<4; i++) {
				LCDFillBox(fg, x, y, x+i, y+j);
				LCDFillBox(fg, x+17, y+1, x+17+i, y+1+j);
				x += i+2;
			}
			y += j+2;
		}

		// test color gradient - horizontal
		x = 36;
		y = 3;
		for(int i=0; i<16; i++) {
			LCDFillBox((fg|bg) & (i*0x111), x+i, y, x+i, y+15);
		}

		// test color gradient - vertical
		x = 54;
		y = 3;
		for(int i=0; i<16; i++) {
			LCDFillBox((fg|bg) & (i*0x111), x, y+i, x+15, y+i);
		}

		// test font and bitmap drawing at various offsets
		char *txt = "NOKIA";
		x = 6;
		y = 18;
		for(int i=0; i<5; i++) {
			drawChar(txt[i], fg, bg, x+i*6, y+i);
			drawChar(txt[4-i], fg, bg, x+24-i, y+4+i*8);
			drawChar(txt[i], fg, bg, x+20-i*6, y+36-i);
			drawChar(txt[4-i], fg, bg, x-4+i, y+32-i*8);
		}

		// Sierpinski triangle, a test of bitmap drawing.
		x = 39;
		y = 20;
		LCDWriteWindow(x, y, x+63, y+63);
		for(int j=0; j<64; j++) {
			for(int i=0; i<64; ) {
				int b1 = (j & i) ? bg : fg;
				i++;
				int b2 = (j & i) ? bg : fg;
				i++;
				LCDWritePair(b1, b2);
			}
		}

		// binary test patten - vertical
		x = 110;
		y = 3;
		LCDWriteWindow(x, y, x+7, y+63);
		for(int j=0; j<64; j++) {
			for(int i=0; i<8;) {
				int b1 = (j & (1<<i)) ? fg : bg;
				i++;
				int b2 = (j & (1<<i)) ? fg : bg;
				i++;
				LCDWritePair(b1, b2);
			}
		}

		// binary test patten - horizontal
		x = 59;
		y = 74;
		LCDWriteWindow(x, y, x+63, y+7);
		for(int j=0; j<8; j++) {
			for(int i=0; i<64;) {
				int b1 = (i & (1<<j)) ? fg : bg;
				i++;
				int b2 = (i & (1<<j)) ? fg : bg;
				i++;
				LCDWritePair(b1, b2);
			}
		}

		// another color gradient
		x = 60;
		y = 93;
		int c0 = (fg|bg);
		int c1 = (c0 << 4) | (0x0f & (c0 >> 8));
		int c2 = (c0 << 8) | (0xff & (c0 >> 4));
		c2 &= ~c1;
		for(int j=0; j<16; j++) {
			for(int i=0; i<16; i++) {
				LCDSetPixel(
					(c1 & (i*0x111)) | (c2 & (j*0x111)),
					x+i, y+j);
			}
		}

		// Checkerboard.  Tests setting of single pixels
		// at various offsets.
		x = 80;
		y = 93;
		for(int j=0; j<7; j++) {
			for(int i=0; i<7; i++) {
				if((i&1) == (j&1)) {
					LCDSetPixel(fg, x+i, y+j);
				}
			}
		}

		// checkerboard using bitmap drawing
		x = 90;
		y = 93;
		LCDWriteWindow(x, y, x+6, y+6);
		for(int i=0; i<24; i++) {
			LCDWritePair(fg, bg);
		}
		// last pixel wraps around to beginning so
		// we need to treat it as a special case
		LCDWritePair(fg, fg);

		// another checkerboard, with inverted colors
		x = 100;
		y = 93;
		LCDWriteWindow(x, y, x+6, y+6);
		for(int i=0; i<24; i++) {
			LCDWritePair(bg, fg);
		}
		// last pixel wraps around to beginning so
		// we need to treat it as a special case
		LCDWritePair(bg, bg);

		// show the hex codes for the colors we are using
		x = 2;
		y = 85;
		char label[14];
		strcpy(label, "fg:xxx bg:xxx");
		char *hexchr = "0123456789ABCDEF";
		label[3]  = hexchr[(fg>>8) & 0xf];
		label[4]  = hexchr[(fg>>4) & 0xf];
		label[5]  = hexchr[(fg   ) & 0xf];
		label[10] = hexchr[(bg>>8) & 0xf];
		label[11] = hexchr[(bg>>4) & 0xf];
		label[12] = hexchr[(bg   ) & 0xf];
		drawString(label, fg, bg, x, y);
		y += FONT_HEIGHT;

		// delay loop... prints an increasing hex number
		// and a binary bar code
		for(long j=0; j<1024; j++) {
			for(int i=0; i<4; i++) {
				label[i] = hexchr[(j>>(4*(3-i))) & 0xf];
			}
			label[4] = 0;
			drawString(label, fg, bg, x, y);
			for(int i=0; i<10; i++) {
				LCDFillBox((j&(1<<i)) ? fg : bg,
					x+i*3, y+FONT_HEIGHT, 
					x+i*3+2, y+FONT_HEIGHT+4);
				LCDFillBox((j&(1<<i)) ? bg : fg,
					x+i*3, y+FONT_HEIGHT+5, 
					x+i*3+2, y+FONT_HEIGHT+9);
			}
		}
	}

	// not really needed because the previous
	// loop never exits...
	for(;;);
}
