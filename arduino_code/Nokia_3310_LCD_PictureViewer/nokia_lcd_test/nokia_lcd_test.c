/* Include statements */
#include <avr/io.h>
#include "Config.h"
#include "3310_routines.h"
#include "uart.h"

unsigned char byte_pixel[504];
int done = 0;
unsigned char df_data;
unsigned char df_seq_msb;
unsigned char df_seq_lsb;
unsigned char df_end;
unsigned char df_cr;
int index = 0;

int main(void)
{
	
   UART_init();
   spi_init();
   delay_ms(100);	
   LCD_init ();
   delay_ms(100);
   LCD_clear ();

   // send the init frame
   //UART_transmit(0xFB);
   UART_transmit(0xFB);
   UART_transmit(0x0A);
   UART_transmit(0x0A);
   UART_transmit(0x0A);
   UART_transmit(0x0D); 

   // upload the pixel array
   done = 0;
   while (done == 0) {
      // receive data frame
	  df_data = UART_receive();
      df_seq_msb = UART_receive();
	  df_seq_lsb = UART_receive();
	  df_end = UART_receive();
	  df_cr = UART_receive();

      // store into byte pixel array
	  if (df_seq_msb == 0x01) {
		byte_pixel[256 + df_seq_lsb] = df_data; 
	  }
	  else {
        byte_pixel[df_seq_lsb] = df_data;
	  }
      
	  // send the ack
	  UART_transmit(0xFA);
	  UART_transmit(df_seq_msb);
	  UART_transmit(df_seq_lsb);
	  UART_transmit(0x0A);
	  UART_transmit(0x0D);
  
      // break when the last byte (503) is received
      if ((df_seq_msb == 0x01) && (df_seq_lsb == 247)) {
        done = 1;
	  }
   }

   // set the cursor
   LCD_gotoXY(0,0);
   // write byte_pixel by byte_pixel
   for (index = 0; index < 504; index++) {
      LCD_writeData(byte_pixel[index]);
   }


/*
   //LCD_drawBorder ( );

   LCD_gotoXY (4,2);
   //LCD_writeChar (0x41);
   //LCD_writeString_F("Thermometer");
   //LCD_writeString_F("0123");
   LCD_writeChar (0x20);
   LCD_writeChar (0x20);
   LCD_writeChar (0x68);
   LCD_writeChar (0x6f);
   LCD_writeChar (0x77);
   LCD_writeChar (0x64);
   LCD_writeChar (0x79); 

   LCD_gotoXY (4,3);
   //LCD_writeChar (0x43);
   //LCD_writeString_F("by DHARMANI");
   //LCD_writeString_F("6789");
   LCD_writeChar (0x44);
   LCD_writeChar (0x61);
   LCD_writeChar (0x6D);
   LCD_writeChar (0x6D);
   LCD_writeChar (0x69);
   LCD_writeChar (0x74);
   LCD_writeChar (0x21);
*/
}
