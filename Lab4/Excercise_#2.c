#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>
#include <stdlib.h>

#define R 0x04
#define G 0x02
#define Y 0x08

//selecting peripheral mode
#define INIT \
do{ \
  P3SEL |= 0x30; \
}while(0)

#define LED_INIT \
do{ \
  P4DIR |= R+G+Y; \
  P4OUT |= R+G+Y; \
  P3SEL |= 0x30; \
}while(0)

struct Buffer{
  uint8_t size;
  uint8_t in;
  uint8_t out;
  uint8_t data[5];
};

struct Buffer* ringbuf= {0};



void push(struct Buffer* ringbuf){
    ringbuf->data[ringbuf->in] = U0RXBUF; 
    ringbuf->size ++;
    ringbuf->in = (ringbuf->in + 1) % 5;
}

void pop(struct Buffer* ringbuf){
    U0TXBUF = ringbuf->data[ringbuf->out];
    ringbuf->size --;
    ringbuf->out = (ringbuf->out + 1) % 5;
}

// TBCCR1-3 interrupt handler
__attribute__((interrupt(TIMERB1_VECTOR))) void timer_handler()
{
	// Accessing TBIV automatically resets the hight pending interrupt flag 
  switch(TBIV)
    {
    case TBIV_TBIFG:
      IE1 |=   UTXIE0;
    }
}

// receive interrupt handler
__attribute__((interrupt(USART0RX_VECTOR))) void receive_handler()
{
  if(ringbuf->size == 5) {
    return;
  } else if(ringbuf->size == 0) {
	// start the clock
	TBCTL = TBSSEL_1 + CNTL_1 + MC_2 + TBIE + ID_3;
    };
    push(ringbuf);
}

// transmit interrupt handler
__attribute__((interrupt(USART0TX_VECTOR))) void transmit_handler()
{
	pop(ringbuf); 
	if(ringbuf->size == 0){
	    IE1 &= ~UTXIE0; // if there is nothing in the buffer dont transmit anything	
	}
}

// Notes for the timer
// adjust word length also the divider 2^12 * 8 || 8 is the divider
// enable interupt for tbifg 

int main()
{
  // disable the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  // Enable maskable interrupts (See intrinsics.h)
  __eint();
  // serial pins to peripheral mode
  INIT;
  // Set SWRST bit
  U0CTL |= SWRST;
  // Char length 8 bits
  U0CTL |= CHAR;
  // Select ACLK clock source
  U0TCTL |= I2CSSEL_1;
  
  // Set baud rate to 2400
  U0BR1 = 0; U0BR0 = 0x0D; U0MCTL = 0x6B;
  // Enable Transmitting and receiving
  ME1 |= URXE0 | UTXE0;
  // Reset SWRST bit
  U0CTL &= ~SWRST;
  // Enable receiver interrupt
  IE1 |=   URXIE0;

  LPM3;		
}
