#include <stdint.h>
#include <msp430.h>
#include <intrinsics.h>

#define INIT \
do{ \
  P3SEL |= 0x30; \
}while(0)

char buf[] = "hello, world";
static uint8_t i = 0;
// transmit interrupt handler
__attribute__((interrupt(USART0TX_VECTOR))) void transmit_handler(){
	
	U0TXBUF = buf[i];
        i=i+1;
	if(i>=12){
	// The following line will stop the sender interrupt after sending the last character
		IE1 &= ~UTXIE0;
	}
}

int main () {
    // disable the watchdog timer
    WDTCTL = WDTPW + WDTHOLD;
    // Enable maskable interrupts (See intrinsics.h)
    __eint();
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
    ME1 |= UTXE0;
    // Reset SWRST bit
    U0CTL &= ~SWRST;
    // Enable Trans interrupt
    IE1 |=   UTXIE0;
     
    LPM3;
}
