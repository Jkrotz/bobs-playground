   #include <avr/io.h>
   #include "usart.h"
   #include <avr/interrupt.h>
   #include <util/delay.h>

   unsigned char pod;

   main() {

		/* set the baud rate */
		uart_9600();

		/*  initialize the UART */
		USART_Init();

		/* set the USART receive interrupt enable */
		UCSR1B |= (1<<RXCIE1);

		/* set global interrupts */
		sei();

		/* set direction of gpio register */
		PORTC |= (1<<PC7);
		DDRC |= (1<<DDC7);


		while(1 == 1)
		{
			/*  no interrupt example */
			//pod = USART_Receive();
			//USART_Transmit(pod);

			PORTC = ~PORTC;
			_delay_ms(1000);

		}
   }

   ISR(USART1_RX_vect)
   {
		pod = UDR1;
		UDR1 = pod;
   }
