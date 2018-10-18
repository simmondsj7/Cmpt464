#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>

#define R 0x04
#define G 0x02
#define Y 0x08



#define LED_INIT \
do{ \
  P4DIR |= R+G+Y; \
  P4OUT |= R+G+Y; \
  P3SEL |= 0x30; \
}while(0)

uint8_t buf;

// receive interrupt handler
__attribute__((interrupt(USART0RX_VECTOR))) void receive_handler()
{
	P4OUT ^= R;
	buf = U0RXBUF;
	IE1 |=   UTXIE0;
}

// transmit interrupt handler
__attribute__((interrupt(USART0TX_VECTOR))) void transmit_handler()
{
	P4OUT ^= G;
	U0TXBUF = buf;
	IE1 &=   ~UTXIE0;
}

int main()
{
  // disable the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  // Enable maskable interrupts (See intrinsics.h)
  __eint();
  // Initialize all leds to be off and serial pins to peripheral mode
  LED_INIT;
  // Set SWRST bit
  U0CTL |= SWRST;
  // Char length 8 bits
  U0CTL |= CHAR;
  // Select ACLK clock source
  U0TCTL |= I2CSSEL_1;

  // Set buad rate to 2400
   U0BR1 = 0; U0BR0 = 0x0D; U0MCTL = 0x6B;
  // Enable Transmitting and receiving
  ME1 |= URXE0 | UTXE0;
  // Reset SWRST bit
  U0CTL &= ~SWRST;
  // Enable receiver interrupt
  IE1 |=   URXIE0;
   
  //
  LPM3;		
}
