#include "PCD8544.h"

//  ORIGINAL
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//PCD8544 nokia = PCD8544(7, 6, 5, 4, 3);

//   MY BOARD
// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 7 - LCD chip select (CS)
// pin 6 - LCD reset (RST)
PCD8544 nokia = PCD8544(3, 4, 5, 7, 6);

// the nokia display is 6 rows by 14 columns for characters

// a bitmap of a 16x16 fruit icon
static unsigned char __attribute__ ((progmem)) logo16_glcd_bmp[]={
0x30, 0xf0, 0xf0, 0xf0, 0xf0, 0x30, 0xf8, 0xbe, 0x9f, 0xff, 0xf8, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 
0x20, 0x3c, 0x3f, 0x3f, 0x1f, 0x19, 0x1f, 0x7b, 0xfb, 0xfe, 0xfe, 0x07, 0x07, 0x07, 0x03, 0x00, };
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

char message[] = "12345678901234567890";
int x_val = 0;
int line_val = 0;

unsigned char the_character;

void setup(void) {
  
  Serial.begin(9600);
  
  Serial.println("hello!");

  nokia.init();

  // turn all the pixels on (a handy test)
  nokia.command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYALLON);
  delay(500);
  // back to normal
  nokia.command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

//  nokia.display();
//  delay(500);
//  nokia.clear();
//  nokia.drawstring(0,0,message);
//  nokia.display();

  // show splashscreen
  nokia.display();
  delay(50);
  nokia.clear();
  
  x_val = 0;
  line_val = 0;
//
//  // draw a single pixel
//  nokia.setPixel(10, 10, BLACK);
  //nokia.drawstring(0,0,message);
//  x_val = 0;
//  if (Serial.available() > 0) {
//    the_character = Serial.read();
//    drawchar(
//    
//  }
//  nokia.display();        // show the changes to the buffer
//  delay(2000);
//  nokia.clear();
// 
//   // draw many lines
//  testdrawline();
//  nokia.display();       // show the lines
// // nokia.clear();
//  
//  // draw rectangles
//  testdrawrect();
//  nokia.display();
//  delay(2000);
//  nokia.clear();
//  
//  // draw multiple rectangles
//  testfillrect();
//  nokia.display();
//  delay(2000);
//  nokia.clear();
//  
//  // draw mulitple circles
//  testdrawcircle();
//  nokia.display();
//  delay(2000);
//  nokia.clear();
//  
//  // draw the first ~120 characters in the font
//  testdrawchar();
//  nokia.display();
//  delay(2000);
//  nokia.clear();
//
//  // draw a string at location (0,0)
//  nokia.drawstring(0, 0, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor");
//  nokia.display();
//  delay(2000);
//  nokia.clear();
//
//  // draw a bitmap icon and 'animate' movement
//  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

void loop(void) {

  if (Serial.available() > 0) {
    the_character = Serial.read();
    if (the_character == 0xAA) {
      line_val = line_val + 1;
      x_val = 0;
    } else if (the_character == 0xAB) {
      nokia.display();        // show the changes to the buffer
      delay(2000);
      nokia.clear();
      line_val = 0; 
      x_val = 0;   
    }
    if (line_val > 6) {
      nokia.display();        // show the changes to the buffer
      delay(2000);
      nokia.clear();      
      line_val = 0;
      x_val = 0;
    }
    if (((the_character != 0xAB) || (the_character != 0xAA)) && (line_val < 6)) {    
      nokia.drawchar(x_val, line_val, the_character);
      //nokia.display();
      x_val = x_val + 8;
    }
  }
//  nokia.display();        // show the changes to the buffer
//  delay(2000);
//  nokia.clear();

}

#define NUMFLAKES 8
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
  srandom(666);     // whatever seed
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random() % LCDWIDTH;
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random() % 5 + 1;
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      nokia.drawbitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
    }
    nokia.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      nokia.drawbitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, 0);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > LCDHEIGHT) {
	icons[f][XPOS] = random() % LCDWIDTH;
	icons[f][YPOS] = 0;
	icons[f][DELTAY] = random() % 5 + 1;
      }
    }
  }
}

void testdrawchar(void) {
  for (uint8_t i=0; i < 64; i++) {
    nokia.drawchar((i % 14) * 6, i/14, i);
  }    
  nokia.display();
  delay(2000);
  for (uint8_t i=0; i < 64; i++) {
    nokia.drawchar((i % 14) * 6, i/14, i + 64);
  }    
}

void testdrawcircle(void) {
  for (uint8_t i=0; i<48; i+=2) {
    nokia.drawcircle(41, 23, i, BLACK);
  }
}


void testdrawrect(void) {
  for (uint8_t i=0; i<48; i+=2) {
    nokia.drawrect(i, i, 96-i, 48-i, BLACK);
  }
}

void testfillrect(void) {
  for (uint8_t i=0; i<48; i++) {
      // alternate colors for moire effect
    nokia.fillrect(i, i, 84-i, 48-i, i%2);
  }
}

void testdrawline() {
  for (uint8_t i=0; i<84; i+=4) {
    nokia.drawline(0, 0, i, 47, BLACK);
  }
  for (uint8_t i=0; i<48; i+=4) {
    nokia.drawline(0, 0, 83, i, BLACK);
  }

  nokia.display();
  delay(1000);
  for (uint8_t i=0; i<84; i+=4) {
    nokia.drawline(i, 47, 0, 0, WHITE);
  }
  for (uint8_t i=0; i<48; i+=4) {
    nokia.drawline(83, i, 0, 0, WHITE);
  }
}