/*
  LiquidCrystal Library - Serial Input
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch displays text sent over the serial port 
 (e.g. from the Serial Monitor) on an attached LCD.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD R/W pin to digital pin 11
 * LCD Enable pin to digital pin 10
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried
 example added 7 Jul 2009
 by Tom Igoe 
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
#include "WProgram.h"
void setup();
void loop();
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

const int numRows = 4;
const int numCols = 20;

void setup(){
    // set up the LCD's number of columns and rows: 
  lcd.begin(numRows, numCols);
  // initialize the serial communications:
  Serial.begin(9600);
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      
      for (int thisCol = 0; thisCol < numRows; thisCol++) {
        // loop over the rows:
        for (int thisRow = 0; thisRow < numCols; thisRow++) {
          // set the cursor position:
          lcd.setCursor(thisRow,thisCol);
          // print the letter:
          //lcd.print(thisLetter, BYTE);
          // display each character to the LCD
          lcd.write(Serial.read());          
          //delay(200);
        }
      }       
      // display each character to the LCD
      //lcd.write(Serial.read());
    }
  }
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

