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

char *morse_string;
// start the index at the beginning of the string
int morse_string_index = 0;

uint8_t idle_flag;

uint8_t first_character_flag =0;

// function that checks the character buffer at the last fill point
// if its a valid character than set the corresponding morse code string
// to the morse string that we will iterate later. Also decrements the 
// position that of the in buffer because that character is no longer needed.
// Also starts the index for the morse string to zero for every case
void buf_to_morse(){
  switch (ringbuf.data[ringbuf.out]){
  case 'a':
    morse_string = m[0].code;
     break;
  case 'b':
    morse_string = m[1].code;
  case 'c':
    morse_string = m[2].code;
  case 'd':
    morse_string = m[3].code;
  case 'e':
    morse_string = m[4].code;
  case 'f':
    morse_string = m[5].code;
  case 'g':
    morse_string = m[6].code;
  case 'h':
    morse_string = m[7].code;
  case 'i':
    morse_string = m[8].code;
  case 'j':
    morse_string = m[9].code;
  case 'k':
    morse_string = m[10].code;
  case 'l':
    morse_string = m[11].code;
  case 'm':
    morse_string = m[12].code;
  case 'n':
    morse_string = m[13].code;
  case 'o':
    morse_string = m[14].code;
  case 'p':
    morse_string = m[15].code;
  case 'q':
    morse_string = m[16].code;
  case 'r':
    morse_string = m[17].code;
  case 's':
    morse_string = m[18].code;
  case 't':
    morse_string = m[19].code;
  case 'u':
    morse_string = m[20].code;
  case 'v':
    morse_string = m[21].code;
  case 'w':
    morse_string = m[22].code;
  case 'x':
    morse_string = m[23].code;
  case 'y':
    morse_string = m[24].code;
  case 'z':
    morse_string = m[25].code;
  case '1':
    morse_string = m[26].code;
  case '2':
    morse_string = m[27].code;
  case '3':
    morse_string = m[28].code;
  case '4':
    morse_string = m[29].code;
  case '5':
    morse_string = m[30].code;
  case '6':
    morse_string = m[31].code;
  case '7':
    morse_string = m[32].code;
  case '8':
    morse_string = m[33].code;
  case '9':
    morse_string = m[34].code;
  case '0':
    morse_string = m[35].code;
  case '.':
    morse_string = m[36].code;
  case '@':
    morse_string = m[37].code;
  case '~':
    morse_string = m[38].code;
  }
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
    return;
  }

 
  U0TXBUF = morse_string[morse_string_index];//morse_string[morse_string_index];
  morse_string_index ++; 
}

// Timer interupt
 __attribute__((interrupt(TIMERB0_VECTOR))) void timer_handler()
 {
IE1 |= UTXIE0;
 if (morse_string_index >= 2){
    ringbuf.size--;
    ringbuf.out = (ringbuf.out + 1) % 32;
	  DOT_ON;
	  morse_string_index = 0;
}
	buf_to_morse();
    
   //DOT_ON;
   switch(morse_string[morse_string_index]){
   case '.':
     TBCCR0 += DOT;
     //IE1 |= UTXIE0;
    

	break;
   case '-':
     TBCCR0 += DASH;
     //IE1 |= UTXIE0;
      break;
   //case '\0':
    //IE1 &= ~UTXIE0;
     //ringbuf.size--;
  //ringbuf.out = (ringbuf.out + 1) % 32;
   }

 
//IE1 |= UTXIE0;
}

// receive interrupt handler
__attribute__((interrupt(USART0RX_VECTOR))) void receive_handler()
{
  // enable the timer
  if(ringbuf.size == 32) { // if ringbuf size is full do nothing
    return;
  } else if (ringbuf.size == 0) {
        TBCCR0 = 1;
        TBCCTL0 = CCIE; 
        TBCTL = TBSSEL_1 + CNTL_0 + MC_2+TBCLR;
	
    }
  
  push();
}

// transmit interrupt handler
__attribute__((interrupt(USART0TX_VECTOR))) void transmit_handler()
{
 P4OUT ^= 0x02;
    pop();
    if(ringbuf.size == 0){
      //TODO: Sending process should be sychronized with the led status change
      //IE1 &= ~UTXIE0; // if there is nothing in the buffer dont transmit anything
  } 

  
IE1 &= ~UTXIE0;
}


int main() {
  //Initialize structure
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
 // Initialize all leds to be off
  LED_INIT;

  LOW_POWER_MODE;
 
  return 0;
}
