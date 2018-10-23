#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>

#define R 0x04
#define G 0x02
#define Y 0x08

#define R_delay (32768/2)
#define G_delay (32768/4)
#define Y_delay (32768/8)
#define ON 3000

#define LED_INIT \
do{ \
  P4DIR |= R+G+Y; \
  P4OUT |= R+G+Y; \
}while(0)

// TBCCR1-3 interrupt handler
__attribute__((interrupt(TIMERB1_VECTOR))) void timer_handler()
{
	// Accessing TBIV automatically resets the hight pending interrupt flag 
	switch(TBIV)
	{
	case TBIV_TBCCR1:
		P4OUT ^= R;
		if(P4OUT & R)
			TBCCR1 += ON;  // Red led on period
		else
			TBCCR1 += (uint16_t)(R_delay - ON); // Red led off period
		break;
	case TBIV_TBCCR2:
		P4OUT ^= G;
		if(P4OUT & G)
			TBCCR2 += ON; // Green led on period
		else
			TBCCR2 += (uint16_t)(G_delay - ON); // Green led off period
		break;
	case TBIV_TBCCR3:
		P4OUT ^= Y;
		if(P4OUT & Y)
			TBCCR3 += ON; // Yellow led on period
		else
			TBCCR3 += (uint16_t)(Y_delay - ON); // Yellow led off period
		break;
	}
}


int main()
{
  // disable the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  // Enable maskable interrupts (See intrinsics.h)
  __eint();
  // Initialize all leds to be off
  LED_INIT;
  // set TBCCR1-3 to trigger the interrupts as soon as possible
  TBCCR1 = TBCCR2 = TBCCR3 = 1;
  // Enable interrupts for TBCCR1-3
  TBCCTL1 = TBCCTL2 = TBCCTL3 = CCIE;   

  // 16 bits, continueous mode, ACLK select
  TBCTL = TBSSEL_1 + CNTL_0 + MC_2 ;
  //
  LPM3;		
}
