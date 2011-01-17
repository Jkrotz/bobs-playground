/*******************************************************************
 * CDT Snuffy                                                      *
 * USART                                                           *
 * 1 October 2008                                                  *
 * EE487                                                           *
 * MAJ Korpela                                                     *
 *                                                                 *
 * File name: uart.h                                               *
 * Description: UART header file                                   *
 *******************************************************************/

/* Include Staatement */
#include "Config.h"

/* UART Settings */
#define UART_BAUD_RATE	9600
#define UART_DATA_BIT_8  (( 1 << UCSZ1 ) | ( 1 << UCSZ0 ))
#define UART_PARITY_NONE (( 0 << UPM1 )  | ( 0 << UPM0 ))
#define UART_STOP_BIT_1   ( 0 << USBS )

#define UBRR_INIT   (( CFG_CPU_CLOCK / 16 / UART_BAUD_RATE ) - 1 )
//#define UBRR_INIT   6


#define UCSRA_INIT  0
#define UCSRB_INIT  (( 0 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN ))
#define UCSRC_INIT  (( 1 << URSEL ) | ( UART_DATA_BIT_8 | UART_PARITY_NONE | UART_STOP_BIT_1 ))

/* Function prototypes */
void UART_init (void);
void UART_transmit (unsigned char);
unsigned char UART_receive (void);

