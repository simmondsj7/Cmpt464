#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>
#include "morse_code.h"
#include <stdlib.h>

//------------------------------------------
// John Simmonds
// Cmpt 464
// October 12th 2018
// Design a program that interacts with MSP430 using UART and
// TimerB and interupts to display morse code to minicom from 
// Keyboard input of characters
//-------------------------------------------

struct Buffer ringbuf= {.in=0, .out=0, .size=0};



void push(){
// max size that im allowing
  if(ringbuf.size==32){
    ringbuf.size = 0;
    ringbuf.in=0;
    ringbuf.out=0;
    return;
  };

  ringbuf.data[ringbuf.in] = U0RXBUF; //issue here
  ringbuf.size++;
  ringbuf.in = (ringbuf.in +1) % 32;
}

// Function that checks if char is one of the letters
// if it is than return the char* of the morse code value
// Returns char array
char* check_character(char c) { 
  switch(c):
    case('a'):
      U0TXBUF = 1;
  return ""; 
}

// Function that checks the ringbuffer that we are using and if the size
// is 0 and the buffer is empty than we return. If not than we check the
// character that is in the buffer, see if the character is
void pop(){
  if(ringbuf.size==0)
    return;
  char* code = check_character(ringbuf.data[ringbuf.out]);
  //U0TXBUF = code;
  ringbuf.size--;
  ringbuf.out = (ringbuf.out + 1) % 32;
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
  if(ringbuf.size == 32) { // if ringbuf size is full
    return;
  } else if(ringbuf.size == 0) {
    // start the clock
    TBCTL = TBSSEL_1 + CNTL_1 + MC_2 + TBIE + ID_3;
    };
    push();
}

// transmit interrupt handler
__attribute__((interrupt(USART0TX_VECTOR))) void transmit_handler()
{
    pop();
    if(ringbuf.size == 0){
        IE1 &= ~UTXIE0; // if there is nothing in the buffer dont transmit anything   
    }
}


int main() {
  //Initialize structure
  INIT_STRUCT;
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

  LOW_POWER_MODE;
  // Initialize all leds to be off
  LED_INIT;
}
