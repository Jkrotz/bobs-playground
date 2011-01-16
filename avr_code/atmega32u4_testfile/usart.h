   #include <avr/io.h>

   #define F_CPU 16000000

   void uart_9600(void);
   void uart_38400(void);
   void USART_Init(void);
   void USART_Transmit( unsigned char data);
   unsigned char USART_Receive(void);
