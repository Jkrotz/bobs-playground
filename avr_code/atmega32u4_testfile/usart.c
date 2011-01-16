   #include <avr/io.h>
   #include "usart.h"

   void uart_9600(void)
   {
   	#define BAUD 9600
   	#include <util/setbaud.h>
   	UBRR1H = UBRRH_VALUE;
   	UBRR1L = UBRRL_VALUE;
   	#if USE_2X
   		UCSR1A |= (1 << U2X1);
   	#else
   		UCSR1A &= ~(1 << U2X1);
   	#endif
   }

   void uart_38400(void)
   {
   	#undef BAUD  // avoid compiler warning
   	#define BAUD 38400
   	#include <util/setbaud.h>
   		UBRR1H = UBRRH_VALUE;
   		UBRR1L = UBRRL_VALUE;
   	#if USE_2X
   		UCSR1A |= (1 << U2X1);
   	#else
   		UCSR1A &= ~(1 << U2X1);
   	#endif
   }

   void USART_Init(void)
   {
		/* enable the receiver and transmitter */
		UCSR1B = (1<<RXEN1) | (1<<TXEN1);
		/* set frame format:  8 data, 1 stop bit */
		UCSR1C = (3<<UCSZ10);
   }

   void USART_Transmit( unsigned char data)
   {
   		/*  Wait for empty transmit buffer */
		while ( !( UCSR1A & (1<<UDRE1)) );

		/* put data into buyffer, sends the data */
		UDR1 = data;
   }


   unsigned char USART_Receive(void)
   {
    	/* wait for data to be received */
		while ( !(UCSR1A & (1<<RXC1)) );

		/* Get and return received data from buffer */
		return UDR1;
   }
