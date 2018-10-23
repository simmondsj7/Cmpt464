#include <msp430.h>
#include <stdint.h>
#include <intrinsics.h>

#define R 0x04
#define G 0x02
#define Y 0x08

#define PERIOD 32768

#define LED_INIT \
do{ \
  P4DIR |= R+G+Y; \
  P4OUT |= R+G+Y; \
}while(0)

// TBCCR0 interrupt handler: Interupt event handler
__attribute__((interrupt(TIMERB0_VECTOR))) void timer_handler()
{
	P4OUT ^= R;
	TBCCR0 += (uint16_t) PERIOD;
}

int main()
{
  // disable the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  // Enable maskable interrupts (See intrinsics.h)
  __eint();
  // Initialize all leds to be off
  LED_INIT;

  //
  
}
