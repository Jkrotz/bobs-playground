#include <Wire.h>
#include <LiquidCrystal.h>


// this constant won't change.  It's the pin number
// of the sensor's output:
const int pingPin = 7;

int HMC6352Address = 0x42;
int slaveAddress;             // This is calculated in the setup() function

int ledPin = 13;
boolean ledState = false;

byte headingData[2];
int i, headingValue;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
{
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);  
  
  // Shift the device's documented slave address (0x42) 1 bit right
  // This compensates for how the TWI library only wants the
  // 7 most significant bits (with the high bit padded with 0)

  slaveAddress = HMC6352Address >> 1;   // This results in 0x21 as the address to pass to TWI
 
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      // Set the LED pin as output
  Wire.begin();
}

void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;  
  
  lcd.clear();
  
  // Flash the LED on pin 13 just to show that something is happening
  // Also serves as an indication that we're not "stuck" waiting for TWI data

  ledState = !ledState;
  if (ledState) {
    digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
 
  // Send a "A" command to the HMC6352
  // This requests the current heading data
 
  Wire.beginTransmission(slaveAddress);
  Wire.send("A");              // The "Get Data" command
  Wire.endTransmission();
  delay(10);                   // The HMC6352 needs at least a 70us (microsecond) delay
                               // after this command.  Using 10ms just makes it safe
 
  // Read the 2 heading bytes, MSB first
  // The resulting 16bit word is the compass heading in 10th's of a degree
  // For example: a heading of 1345 would be 134.5 degrees

  Wire.requestFrom(slaveAddress, 2);        // Request the 2 byte heading (MSB comes first)
  i = 0;
  while(Wire.available() && i < 2)
  { 
    headingData[i] = Wire.receive();
    i++;
  }

  headingValue = headingData[0]*256 + headingData[1];  // Put the MSB and LSB together
  //Serial.print("Current heading: ");
  Serial.print(int (headingValue / 10));     // The whole number part of the heading
  Serial.print("\r\n");
  //Serial.print(".");
  //Serial.print(int (headingValue % 10));     // The fractional part of the heading
  //Serial.println(" degrees");
 
  lcd.print(int (headingValue/10));

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
 
  lcd.print("     ");
  lcd.print(inches);
 
  delay(500);
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
