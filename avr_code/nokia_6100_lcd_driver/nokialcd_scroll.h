// the toparea_height, scrollarea_height, and delta parameters must
// all be divisible by four!
void scrollInit(int toparea_height, int scrollarea_height);
void scrollShift(int delta);
int scrollConvertY(int y);
