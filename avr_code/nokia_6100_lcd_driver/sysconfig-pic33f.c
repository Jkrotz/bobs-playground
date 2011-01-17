#include <p33Fxxxx.h>

_FOSCSEL(FNOSC_PRIPLL & IESO_OFF);
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_HS);
_FWDT(FWDTEN_OFF);
_FICD(JTAGEN_OFF);

#define LCDCS_TRIS    TRISBbits.TRISB15
#define LCDCS         LATBbits.LATB15
#define LCDSCK_TRIS   TRISBbits.TRISB14
#define LCDSCK        LATBbits.LATB14
#define LCDMOSI_TRIS  TRISBbits.TRISB13
#define LCDMOSI       LATBbits.LATB13
#define LCDMISO_TRIS  TRISBbits.TRISB12
#define LCDMISO       PORTBbits.RB12
#define LCDRST_TRIS   TRISBbits.TRISB11
#define LCDRST        LATBbits.LATB11

static inline void bitsleep() {
    asm volatile ("nop");
    asm volatile ("nop");
}

void LCDSend9Bit(int data) {
	LCDCS = 0;

	// You can add some calls to bitsleep in this
	// loop to slow down communication if you
	// have problems.
	// Anyone want to write this in assembly?
	for(int j=9; j; j--) {
        LCDMOSI = (data >> 8) & 1;
		// bitsleep();
		LCDSCK = 0;
        data <<= 1;
		// bitsleep();
		LCDSCK = 1;
    }

	LCDCS = 1;
}

static void LCDInitIO() {
	LCDSCK_TRIS=0;
	LCDMISO_TRIS=1;
	LCDMOSI_TRIS=0;
	LCDCS_TRIS = 0;
	LCDRST_TRIS = 0;
	
	LCDSCK = 0;
	LCDMOSI = 0;
	LCDCS = 1;
	LCDRST = 0;
	for(long j=0; j<300000; j++) asm volatile ("nop");
	LCDRST = 1;
	LCDSCK = 1;
	LCDMOSI = 1;
	for(long j=0; j<300000; j++) asm volatile ("nop");
}

static void ClockInit() {
	// clock
	PLLFBD = 30;	// M = 32
	CLKDIVbits.PLLPOST = 0;	// N1 = 2
	CLKDIVbits.PLLPRE = 0;	// N2 = 2
	OSCTUN = 0;
	RCONbits.SWDTEN = 0;
}

void sysInit() {
    AD1PCFGL = 0xFFFF; // set all pins to digital
	ClockInit();
	LCDInitIO();
}
