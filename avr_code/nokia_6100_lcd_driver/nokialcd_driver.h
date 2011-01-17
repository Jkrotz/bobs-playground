// FIXME - these depend upon the vendor
// correction factor that modifies operation of LCDSetWindow
#define LCD_Y_OFFSET 2
// number of visible pixels
#define LCD_WIDTH 130
#define LCD_HEIGHT 130

void LCDInitController();
void LCDWriteWindow(int x1, int y1, int x2, int y2);
void LCDWritePair(int c1, int c2);
void LCDSetPixel(int color, int x, int y);
void LCDFillBox(int color, int x1, int y1, int x2, int y2);
void LCDClearScreen(int color);

// 12-Bit Color Definitions
#define WHITE	0xFFF
#define BLACK	0x000
#define RED		0xF00
#define GREEN	0x0F0
#define BLUE	0x00F
#define CYAN	0x0FF
#define MAGENTA	0xF0F
#define YELLOW	0xFF0
#define BROWN	0xB22
#define ORANGE	0xFA0
#define PINK	0xF6A
