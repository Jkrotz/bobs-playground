/*******************************************************************
 * CDT Snuffy                                                      *
 * USART and Parallel IO                                           *
 * 1 October 2008                                                  *
 * EE487                                                           *
 * MAJ Korpela                                                     *
 *                                                                 *
 * File name: uart.c                                               *
 * Description: Parallel IO example                                *
 *******************************************************************/

/* Include statements */
#include <avr/io.h>
#include "uart.h"

/*******************************************************************/
/* Functions */
/*******************************************************************/

void UART_init (void)
{
	/* Set Baudrate to 9600 for an 8 Mhz clock */
	/* Refer to Page 169-172 in ATmega16 Databook */
	UBRRH = UBRR_INIT >> 8;
	UBRRL = UBRR_INIT & 0xFF;
	//UBRRL = UBRR_INIT;
	//UBRRH = UBRR_INIT & 0xF0;

	/* Enable receiver and transmitter */
	/* Page 166 */
    UCSRA = UCSRA_INIT;
    UCSRB = UCSRB_INIT;
    UCSRC = UCSRC_INIT;
}

/*******************************************************************/

void UART_transmit (unsigned char data)
{
	while (!(UCSRA & (1<<UDRE)));
	UDR = data;
}

/*******************************************************************/

unsigned char UART_receive (void)
{
	while (!(UCSRA & (1<<RXC)));
	return (UDR);
}

/*******************************************************************/
