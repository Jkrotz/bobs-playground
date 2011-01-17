#include <avr/io.h>

#define LCD_PORT   PORTD
#define LCD_PIN    PIND
#define LCD_DDR    DDRD
#define LCDCS      (1 << 0)
#define SPICLK     (1 << 1)
#define SPIMOSI    (1 << 2)
#define SPIMISO    (1 << 3)
#define LCDRST     (1 << 4)

static inline void bitsleep() {
    asm volatile ("nop");
    asm volatile ("nop");
}

void LCDSend9Bit(int data) {
	LCD_PORT &= ~LCDCS;

	// you can add some calls to bitsleep
	// to slow down communication if you
	// have problems

	// we break the 9th bit out into this special case
	// so that we can use only 8-bit variables in the
	// main loop
	if(data & 0x100) {
		LCD_PORT |= SPIMOSI;
	} else {
		LCD_PORT &= ~SPIMOSI;
	}
	//bitsleep();
	LCD_PORT &= ~SPICLK;
	unsigned char dbyte = data;
	//bitsleep();
	LCD_PORT |= SPICLK;

	// anyone want to write this in assembly?
	for(unsigned char mask=0x80; mask; ) {
        if(dbyte & mask) {
			LCD_PORT |= SPIMOSI;
		} else {
			LCD_PORT &= ~SPIMOSI;
		}
		//bitsleep();
		LCD_PORT &= ~SPICLK;
		mask >>= 1;
		//bitsleep();
		LCD_PORT |= SPICLK;
    }

	LCD_PORT |= LCDCS;
	//bitsleep();
}

static void LCDInitIO() {
	LCD_DDR |= (SPICLK | SPIMOSI | LCDCS | LCDRST);
	LCD_DDR &= ~SPIMISO;
	
	LCD_PORT &= ~SPICLK;
	LCD_PORT &= ~SPIMOSI;
	LCD_PORT |= LCDCS;
	LCD_PORT &= ~LCDRST;

	for(long j=0; j<300000; j++) asm volatile ("nop");

	LCD_PORT |= LCDRST;
	LCD_PORT |= SPICLK;
	LCD_PORT |= SPIMOSI;

	for(long j=0; j<300000; j++) asm volatile ("nop");
}

void sysInit() {
	LCDInitIO();
}
