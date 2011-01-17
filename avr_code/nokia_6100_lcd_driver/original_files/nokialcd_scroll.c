#include "nokialcd_controlcodes.h"
#include "nokialcd_driver.h"
#include "nokialcd_io.h"
#include "nokialcd_scroll.h"

// the toparea_height, scrollarea_height, and delta parameters must
// all be divisible by four!

static int r1, r2, pos;

#ifdef EPSON
// FIXME - need phillips scroll
void LCDSetScrollArea(int n1, int n2, int n3, int mode) {
	LCDCommand(EP_ASCSET);
	LCDData(n1);
	LCDData(n2);
	LCDData(n3);
	LCDData(mode);
}

void LCDSetScrollStart(int n) {
	LCDCommand(EP_SCSTART);
	LCDData(n);
}
#endif

void scrollInit(int toparea_height, int scrollarea_height) {
	r1 = toparea_height / 4;
	r2 = r1 + scrollarea_height / 4;
	pos = r1;

	LCDSetScrollArea(r1, r2-1, r2-1, 0);
	LCDSetScrollStart(pos);
}

void scrollShift(int delta) {
	delta /= 4;
	pos += delta;
	while(pos >= r2) pos -= r2 - r1;
	while(pos <  r1) pos += r2 - r1;
	LCDSetScrollStart(pos);
}

int scrollConvertY(int y) {
	int low = y & 3;
	y = (y >> 2) + pos;
	while(y >= r2) y -= r2 - r1;
	return (y<<2) + low;
}
