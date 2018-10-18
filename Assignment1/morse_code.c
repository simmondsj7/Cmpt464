#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>
#include "morse_code.h"

//------------------------------------------
// John Simmonds
// Cmpt 464
// October 12th 2018
// Design a program that interacts with MSP430 using UART and
// TimerB and interupts to display morse code to minicom from 
// Keyboard input of characters
//-------------------------------------------

struct Buffer* ringbuf= {0};



void push(struct Buffer* ringbuf){
    ringbuf->data[ringbuf->in] = U0RXBUF; 
    ringbuf->size ++;
    ringbuf->in = (ringbuf->in + 1) % 64;
}

void pop(struct Buffer* ringbuf){
    U0TXBUF = ringbuf->data[ringbuf->out];
    ringbuf->size --;
    ringbuf->out = (ringbuf->out + 1) % 64;
}

int main() {
  // disable the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  // Enable maskable interrupts (See intrinsics.h)
  __eint();

  LOW_POWER_MODE;
  // Initialize all leds to be off
  LED_INIT;
}
