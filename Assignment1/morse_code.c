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

char* morse_string;
uint8_t morse_string_index;

uint8_t idle_flag;

// function that checks the character buffer at the last fill point
// if its a valid character than set the corresponding morse code string
// to the morse string that we will iterate later. Also decrements the 
// position that of the in buffer because that character is no longer needed.
// Also starts the index for the morse string to zero for every case
void buf_to_morse(){
  morse_string_index = 0;
  switch (ringbuf.data[ringbuf.in]){
    case 'a':
      morse_string = morse[0].code;
    case 'b':
      morse_string = morse[1].code;
  }
  ringbuf.in = (ringbuf.in - 1) % 32;
}


void push(){
// max size that im allowing. if the buffer is full do nothing
  if(ringbuf.size==32){
    return;
  }else if(ringbuf.size>=0){
    // if the buffer is empty of greater than 0
    ringbuf.data[ringbuf.in] =U0RXBUF;
    ringbuf.size++;
    ringbuf.in = (ringbuf.in +1) % 32;
    return;
  } 
}

// Function that checks the ringbuffer that we are using and if the size
// is 0 and the buffer is empty than we return. If not than we check the
// character that is in the buffer, see if the character is 
void pop(){
  if(ringbuf.size==0) {
    // turns clock off
    TBCTL = MC_0;
  }
  // make the tranmitter buffer equal to the character that is in the morse_string char*
  U0TXBUF = morse_string[morse_string_index];
  ringbuf.size--;
  ringbuf.out = (ringbuf.out + 1) % 32;
}

// TBCCR1-3 interrupt handler
__attribute__((interrupt(TIMERB0_VECTOR))) void timer_handler()
{
    //TODO: use the buf_to_morse function here I think and also move the global pointer 
    //      to the index of the next morse character 
    
}

// receive interrupt handler
__attribute__((interrupt(USART0RX_VECTOR))) void receive_handler()
{
  if(ringbuf.size == 32) { // if ringbuf size is full do nothing
    return;
  } else if(ringbuf.size == 0) {
    //First time that you enter the buffer enable the timer
    TBCTL = TBSSEL_1 + CNTL_0 + MC_2 ;
    push();
    };
    push();
}

// transmit interrupt handler
__attribute__((interrupt(USART0TX_VECTOR))) void transmit_handler()
{
    pop();
    if(ringbuf.size == 0){
      //TODO: Sending process should be sychronized with the led status change
        IE1 &= ~UTXIE0; // if there is nothing in the buffer dont transmit anything   
    }
}


int main() {
  //Initialize structure
  INIT_MORSE_STRUCT;
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
  IE1 |= URXIE0;

  LOW_POWER_MODE;
  // Initialize all leds to be off
  LED_INIT;
}
