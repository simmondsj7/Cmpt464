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
int global_length = 0;
int end_transmit = 0;
int start_transmit =0;
int wait_stage = 0; // 1 is between DOT or dash in morse
                    // 2 is between Letters

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
    global_length = m[0].length;
    break;
  case 'b':
    morse_string = m[1].code;
    global_length = m[1].length;
    break;
  case 'c':
    morse_string = m[2].code;
    global_length = m[2].length;
    break;
  case 'd':
    morse_string = m[3].code;
    global_length = m[3].length;
    break;
  case 'e':
    morse_string = m[4].code;
    global_length = m[4].length;
    break;
  case 'f':
    morse_string = m[5].code;
    global_length = m[5].length;
    break;
  case 'g':
    morse_string = m[6].code;
    global_length = m[6].length;
    break;
  case 'h':
    morse_string = m[7].code;
    global_length = m[7].length;
    break;
  case 'i':
    morse_string = m[8].code;
    global_length = m[8].length;
    break;
  case 'j':
    morse_string = m[9].code;
    global_length = m[9].length;
    break;
  case 'k':
    morse_string = m[10].code;
    global_length = m[10].length;
    break;
  case 'l':
    morse_string = m[11].code;
    global_length = m[11].length;
    break;
  case 'm':
    morse_string = m[12].code;
    global_length = m[12].length;
    break;
  case 'n':
    morse_string = m[13].code;
    global_length = m[13].length;
    break;
  case 'o':
    morse_string = m[14].code;
    global_length = m[14].length;
    break;
  case 'p':
    morse_string = m[15].code;
    global_length = m[15].length;
    break;
  case 'q':
    morse_string = m[16].code;
    global_length = m[17].length;
    break;
  case 'r':
    morse_string = m[17].code;
    global_length = m[17].length;
    break;
  case 's':
    morse_string = m[18].code;
    global_length = m[18].length;
    break;
  case 't':
    morse_string = m[19].code;
    global_length = m[19].length;
    break;
  case 'u':
    morse_string = m[20].code;
    global_length = m[20].length;
    break;
  case 'v':
    morse_string = m[21].code;
    global_length = m[21].length;
    break;
  case 'w':
    morse_string = m[22].code;
    global_length = m[22].length;
    break;
  case 'x':
    morse_string = m[23].code;
    global_length = m[23].length;
    break;
  case 'y':
    morse_string = m[24].code;
    global_length = m[24].length;
    break;
  case 'z':
    morse_string = m[25].code;
    global_length = m[25].length;
    break;
  case '1':
    morse_string = m[26].code;
    global_length = m[26].length;
    break;
  case '2':
    morse_string = m[27].code;
    global_length = m[27].length;
    break;
  case '3':
    morse_string = m[28].code;
    global_length = m[28].length;
    break;
  case '4':
    morse_string = m[29].code;
    global_length = m[29].length;
    break;
  case '5':
    morse_string = m[30].code;
    global_length = m[30].length;
    break;
  case '6':
    morse_string = m[31].code;
    global_length = m[31].length;
    break;
  case '7':
    morse_string = m[32].code;
    global_length = m[32].length;
    break;
  case '8':
    morse_string = m[33].code;
    global_length = m[33].length;
    break;
  case '9':
    morse_string = m[34].code;
    global_length = m[34].length;
    break;
  case '0':
    morse_string = m[35].code;
    global_length = m[35].length;
    break;
  case '.':
    morse_string = m[36].code;
    global_length = m[36].length;
    break;
  case '@':
    morse_string = m[37].code;
    global_length = m[37].length;
    break;
  case '~':
    morse_string = m[38].code;
    global_length = m[38].length;
    break;
  // if you encounter a space as input
  case ' ':
    morse_string = m[39].code;
    global_length = m[39].length;
    break;
  }
}


void push(){
// max size that im allowing. if the buffer is full do nothing
  if(ringbuf.size==32){
    return;
  }else if(ringbuf.size>=0){
    if (ringbuf.size == 0 && start_transmit ==1){
      TIMER_ON;
      ringbuf.data[ringbuf.in] = '~';
      ringbuf.size++;
      ringbuf.in = (ringbuf.in +1) % 32;
      start_transmit =0;
    } 
    // if the is 0 add the first character after the start sign
    ringbuf.data[ringbuf.in] =U0RXBUF;
    ringbuf.size++;
    ringbuf.in = (ringbuf.in +1) % 32;
    return;
    
  } /*else if (ringbuf.size > 0){
    ringbuf.data[ringbuf.in] =U0RXBUF;
    ringbuf.size++;
    ringbuf.in = (ringbuf.in +1) % 32;
    return;
  } */  
}

// Function that checks the ringbuffer that we are using and if the size
// is 0 and the buffer is empty than we return. If not than we check the
// character that is in the buffer, see if the character is 
void pop(){
  U0TXBUF = morse_string[morse_string_index];
  morse_string_index ++; 
}

// Timer interupt
 __attribute__((interrupt(TIMERB0_VECTOR))) void timer_handler()
 {
  buf_to_morse();
  switch (wait_stage){
  case 0:
    if (morse_string_index >= global_length){
      ringbuf.size--;
      ringbuf.out = (ringbuf.out + 1) % 32;
	    morse_string_index = 0;
      wait_stage = 2;
      if (ringbuf.size == 0 ){              // if you pop the last character in the buffer
        ringbuf.data[ringbuf.in] ='@';      // add the end of work variable
        ringbuf.size++;                     // increase the size of the buffer
        ringbuf.in = (ringbuf.in +1) % 32;  
        wait_stage = 0;                     // let the @ variable be treated as any other character
        end_transmit = 1;                   // mark the end of transmit
        return;
      }
    } else {  
      switch(morse_string[morse_string_index]){
        case '.':
          // when the morse_code is outputing a dot wait 120 ms and transmit DOT
          TBCCR0 += DOT;   // add 120 ms to TBCCR0
          IE1 |= UTXIE0;   // enable UART transmit interupts
          P4OUT &= ~R;     // Turn Red led on
          wait_stage = 1;  // Go wait for 120ms and turn Red led off
          break;
        case '-':
         // Morse Code is a DASH output dash for 360 ms and
          TBCCR0 += DASH; // add 360ms to TBCCR0
          IE1 |= UTXIE0;  // enable UART transmit interupts 
          P4OUT &= ~R;    // Turn Red led on
          wait_stage =1;  // Go wait for 120ms and turn Red led off
          break;
        case ' ':
          TBCCR0 += WORD_SPACE; // Wait for 840ms between 2 words
          IE1 |= UTXIE0;        // enable UART transmit interupts
          P4OUT |= R;           // turn off the Red led
          break;
        case '@':
          TBCCR0 += WORD_SPACE; // Wait for 840ms at the end
          IE1 |= UTXIE0;        // enable UART transmit for the end of work
          P4OUT |= R;           // turns off the Red led
          break;
        }
      }
    break;
  case 1:
    TBCCR0 += DOT;            // space between the signals forming the same letter
    P4OUT |= R;               // turn off the Red led
    wait_stage = 0;           // Go back to the normal state
    break;
  case 2:
    TBCCR0 += LETTER_SPACE;   // space between two letters 
    P4OUT |= R;               // turn off the Red led
    wait_stage = 0;           // Go back to the normal state
    break;
  }

  
}

// receive interrupt handler
__attribute__((interrupt(USART0RX_VECTOR))) void receive_handler()
{
  
  if (end_transmit ==1){
    end_transmit =0;
    TIMER_ON;
  }
  // enable the timer
  if(ringbuf.size == 32) { // if ringbuf size is full do nothing
    return;
  } else if (ringbuf.size ==0){
    start_transmit =1;
  }
  push();
  
  
}

// transmit interrupt handler
__attribute__((interrupt(USART0TX_VECTOR))) void transmit_handler()
{ 
    pop();
    if(end_transmit == 1 && morse_string_index == global_length){
      LED_INIT;
      ringbuf.size --;
      TIMER_OFF;
      
    } 
// Disable the Transmit interupt
    IE1 &= ~UTXIE0;
}


int main() {
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
