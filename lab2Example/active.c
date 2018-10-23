#include <msp430.h>
#include <stdint.h>

#define LED_GREEN	0x02 // 0000 0010
#define LED_YELLOW	0x08 // 0000 1000
#define LED_RED		0x04 // 0000 0100
#define LED_ALL		(LED_GREEN + LED_YELLOW + LED_RED) // 0000 1110 socan turn them on and off in one line

#define PIN_INIT do {				\
    P4SEL &= ~(LED_ALL);			\
    P4DIR |= (LED_ALL);				\
    P4OUT &= ~(LED_ALL);			\
    						\
    P1DIR |= 0x01;  /* RSI 0000 0001*/			\
    P1DIR |= 0x02;  /* RSCLK 0000 0010*/			\
    P1DIR &= ~0x04; /* RSO 0000 0100*/			\
    P1DIR |= 0x20;  /* CSn 0010 0000*/			\
  } while (0)
#define LED_ON(led)	do {			\
    P4OUT &= ~(led);				\
  } while (0)
#define LED_OFF(led)	do {			\
    P4OUT |= (led);				\
  } while (0)


#define PIN_RSI		0x01
#define RSI_LOW		do {			\
    P1OUT &= ~(PIN_RSI);			\
  } while (0)
#define RSI_HIGH	do {			\
    P1OUT |= (PIN_RSI);				\
  } while (0)

#define PIN_RSCLK	0x02
#define RSCLK_LOW	do {			\
    P1OUT &= ~(PIN_RSCLK);			\
  } while (0)
#define RSCLK_HIGH	do {			\
    P1OUT |= (PIN_RSCLK);			\
  } while (0)

#define PIN_SO		0x04

#define PIN_CSn		0x20
#define CSn_LOW		do {			\
    P1OUT &= ~(PIN_CSn);			\
  } while (0)
#define CSn_HIGH	do {			\
    P1OUT |= (PIN_CSn);				\
  } while (0)

void send_cc1100_bits(uint8_t value, uint8_t bits)
{
  while (bits) {
    RSCLK_LOW;
    if (value & (0x01 << (bits - 1))) {
      RSI_HIGH;
    } else {
      RSI_LOW;
    }

    __delay_cycles(1);

    RSCLK_HIGH;
    __delay_cycles(1);

    bits--;
  }

  RSCLK_LOW;  
}

int main()
{
  // disable the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;

  PIN_INIT;
  	
  //while(1);		// total consumption CPU ON, LEDs on, Radio ON: 5.48 mA
  //LPM0;			// total consumption CPU OFF, LEDs on: 5.2 mA

  LED_OFF(LED_ALL);
  //while(1); 		// total consumption CPU ON, LEDs off, Radio ON: 2.1 mA
  //LPM0;		// total consumption CPU off, LEDs off, Radio ON: 1.82 mA

  CSn_LOW;
  while (P1IN & PIN_SO);	/* wait for SO to go low */

  // strobe 0x39 and set CSn high
  // transfer single header byte (no data)
  // send R/W bit, burst access bit set to 0, and the six address bits
  send_cc1100_bits(0x39, 8);
  CSn_HIGH;
  
  //LED_OFF(LED_ALL);	// total consumption CPU off, LEDs off, Radio off: 156 microA
  
  LPM0;		// total consumption CPU off, LEDs off, Radio off: 156 microA



  }
