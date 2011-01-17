import processing.serial.*;  // import the serial lib

PImage source;       // Source image
PImage destination;  // Destination image

Serial myPort;  // the serial port
boolean madeContact = false;  // whether there's been serial data in
byte[] pixel_array = new byte[504];
int pixel_index;



void setup() {
  size(200, 200);
  source = loadImage("bob.jpg");  
  // Load the source image and resize it to the same size as the LCD display
  source.loadPixels();
  source.resize(132,132);
  // The destination image is created as a blank image the same size as the source.
  destination = createImage(source.width, source.height, RGB);  
  destination.loadPixels();
 
  // Set the threshold for a pixel on or off
  float threshold = 127; 
  
  // setting the pixels in the destination image
  //   this is just so it can be displayed in a 
  //   seperate window on this workstation
  for (int x = 0; x < source.width; x++) {
    for (int y = 0; y < source.height; y++ ) {
      int loc = x + y*source.width;
      // Test the brightness against the threshold
      destination.pixels[loc] = source.pixels[loc];
//      if (brightness(source.pixels[loc]) > threshold) {
//        destination.pixels[loc]  = color(255);  // White
//      }  else {
//        destination.pixels[loc]  = color(0);    // Black
//      }
    }
  }  

  // populating the pixel array in the same organization
  //   that can be transmitted to the LCD display.
  // initialize the array index
  pixel_index = 0;
//  for (int x = 0; x < source.width; x++)  {
//    for (int row = 0; row < ((source.height)/8); row++)  {
//  for (int row = 0; row < ((source.height)/8); row++)  {
//    for (int x = 0; x < source.width; x++)  {
//       pixel_array[pixel_index] = 0x00;
//       for (int y = row*8; y < ((row*8)+8); y++) {
//        // c is every character row (vertical byte)
//        // need every value of y in this loop to represent a byte top to bottom
//          int pic_index = x + y*source.width;
//          println("pic_index = " + str(pic_index) + "  x=" + str(x) + "  y=" + str(y) + "  row=" + str(row) + "  pixel_index=" + str(pixel_index)); 
//          if (brightness(source.pixels[pic_index]) > threshold) {
//            int pixel_int =  int(pixel_array[pixel_index]) >> 1;  // White shift right a zero
//            pixel_array[pixel_index] = byte(pixel_int);
//            println("      zero");
//          }  else {
//            int pixel_int =  int(pixel_array[pixel_index]) >> 1 | 0x80;  // Black shift right a one
//            pixel_array[pixel_index] = byte(pixel_int);            
//            println("      one");
//          }        
//       }              
//       // then increment the array index
//       println("  pixel_array[" + str(pixel_index) + "]=" + pixel_array[pixel_index]);
//       pixel_index++;
//    } 
//  }
  
  // troubleshooting 
  println("pixel_index = " + pixel_index);

  // We changed the pixels in destination, now update them
  destination.updatePixels();
  // Display the destination image in another window
  image(destination,0,0);
  
  // list all of the available serial COM ports
  //   .. so you can figure out which one you need to use
  println(Serial.list());
  
  // Open whatever port is the one you're using.
  //   NOTE:  could just set this to COM1 and force them to communicate through that.
  myPort = new Serial(this, Serial.list()[0], 9600);
  
  // only generate a serial event when you get a return char:
  myPort.buffer(5);  
  println("reset the microcontroller");
  
}

void draw() {  
  
  // just looping keeping the program executing

}

/*************************************************
*
*  FROM THE MICROCONTROLLER (stick to 5 byte messages period)
*
*      INIT FRAME
*          FIRST BYTE:  0xFB (INIT)
*          SECOND BYTE:  0x0A ('\n') END OF THE  FRAME
*          THIRD BYTE:  0x0A ('\n')
           FOURTH BYTE:  0x0A
*          FIFTH BYTE:  0x0D ('\r') GENERATES THE SERIAL EVENT
*
*      ACK FRAME
*          FIRST BYTE:  OxFA (ACK)
*          SECOND BYTE:  0x__  (MSB SEQUENCE NUMBER OF LAST PIXEL BYTE RECEIVED)
*          THIRD BYTE:  0x__  (LSB SEQUENCE NUMBER OF LAST PIXEL BYTE RECEIVED)
*          FOURTH BYTE:  0x0A ('\n')   END OF THE FRAME
*          FIFTH BYTE:  0x0D ('\r') GENERATES THE SERIAL EVENT
*
*
*   TO THE MICROCONTROLLER
*      DATA FRAME
*          FIRST BYTE:  0x__  (pixel_array[seq_num])
*          SECOND BYTE:  0x__  (MSB SEQUENCE NUM)
*          THIRD BYTE:  0x__   (LSB SEQUENCE NUM)
*          FOURTH BYTE:  0x0A
*          FIFTH BYTE:  0XOD ('\r') END OF THE DATA FRAME
*
*************************************************/


// serialEvent  method is run automatically by the Processing applet
// whenever the buffer reaches the  byte value set in the bufferUntil() 
// method in the setup():
void serialEvent(Serial myPort) { 
  // if serialEvent occurs at all, contact with the microcontroller
  // has been made:
  madeContact = true;
  
  // read the serial buffer [up til '\n' is seen, max 8 bytes]:
  byte[] inBuffer = new byte[5];
  int num_bytes = myPort.readBytes(inBuffer);
  
  println(str(num_bytes) + " bytes read into inBuffer");
  
  int pix_index = 0;
 
  // troubleshooting
  println(inBuffer);
  
  //  if you got any bytes other than the linefeed:
  if (inBuffer != null) {
    // if you received all the sensor strings, use them:
//    if (inBuffer.length >= 2) {
//
//      }
     // init frame received?
     if (int(inBuffer[0]) == 0xFB) {
        // send the first pixel in the array
        myPort.write(pixel_array[0]);
        // then the index (seq num) being sent
        myPort.write(byte(0x00)); // MSB     
        myPort.write(byte(0x00)); // LSB
        myPort.write(byte(0x0A)); // '\n'
        println("sending pixel_array[0]");
     }
     // ack frame received? 
     else if (int(inBuffer[0]) == 0xFA) {
        // calc the LSB
        if ((inBuffer[2] & byte(0x80)) == 0x00) {
          pix_index = int(inBuffer[2]);
        }  
        else {
          pix_index = int(inBuffer[2] & byte(0x7F)) + 128; 
        }
        // take into account the MSB
        if ((inBuffer[1] & byte(0x01)) == 0x01) {
          pix_index = pix_index + 256; 
        }
        // send the next pixel in the array (based upon the seq number in the ACK)
        // if the last ACK received (503), then we are done
        pix_index = pix_index + 1;
        if (pix_index < 504) {
          myPort.write(pixel_array[pix_index]);
          // then the index (seq num) being sent
          if (pix_index < 128) {
            myPort.write(byte(0x00));  // MSB
            myPort.write(byte(inBuffer[2] + 1)); // LSB
          } 
          else if ((pix_index > 127) && (pix_index < 256)) {
            myPort.write(byte(0x00));  // MSB
            myPort.write(byte(0x80) | byte(pix_index - 128)); // LSB
          }  
          else {
            // pix_index > 255
            myPort.write(byte(0x01)); // MSB
            if ((pix_index - 256) < 128) {
              myPort.write(byte(inBuffer[2] + 1)); // LSB
            } 
            else if (((pix_index - 256) > 127) && ((pix_index - 256) < 256)) {
              myPort.write(byte(0x80) | byte((pix_index - 256) - 128)); // LSB    
            }        
          }
          myPort.write(byte(0x0A));  // '\n'          
          println(" sending pixel_array[" + str(pix_index) + "]");
        }
        else {
          println("WERE DONE");
        }
     }
     else {
      println("UNKNOWN FRAME");
     }

   // send out the serial port to ask for an ACK:
   myPort.write('\r');
  }
}


