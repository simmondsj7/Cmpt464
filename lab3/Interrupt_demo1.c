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

typedef struct{
  uint8_t = color;
  uint16_t = time;
} State;


uint8_t state = 0;
struct State Schedule[6];

void iterate_schedule(uint16_t state){
  P4OUT = Schedule[state].color;
  TBCCR0 += Schedule[state].time;
}

// TBCCR0 interrupt handler: Interupt event handler
__attribute__((interrupt(TIMERB0_VECTOR))) void timer_handler()
{
  for(i = 0; i < 6; i ++){
    iterate_schedule(i);
  }
}



int main()
{
  //Dividing each by 4 because we are using the ID bit to 3
  //5ms for the clock
  uint16_t 5ms = (uint16_t)(PERIOD * 0.005)/4;
  //495ms for the clock
  uint16_t 495ms = (unint16_t)(PERIOD * 0.495)/4;
  //3000ms for the clock
  uint16_t 3000ms = (uint16_t)(PERIOD * 3)/4;
  
  Schedule[0].color = P4OUT |= G; // turn Green LED on
  Schedule[0].time = 5ms; // wait for 5ms
  Schedule[1].color = P4OUT &= ~G; // turn Green LED off
  Schedule[1].time = 495ms; //wait for 495ms
  Schedule[2].color = P4OUT |= Y; //turn yellow LED on
  Schedule[2].time = 5ms; // wait for 5ms
  Schedule[3].color = P4OUT &= ~Y; // turn off the yellow LED
  Schedule[3].time = 495ms; //wait for 495ms
  Schedule[4].color = P4OUT |= R; // turn Red LED on
  Schedule[4].time = 5ms; // wait for 5ms
  Schedule[5].color = P4OUT &= ~R; // turn Red LED off
  Schedule[5].time = 3000ms = 3000ms;
  
  
  // disable the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  // Enable maskable interrupts (See intrinsics.h)
  __eint();
  // Initialize all leds to be off
  LED_INIT;
  // set TBCCR0 to trigger the interrupt as soon as possible, Would like the event to happen after 1 clock cycle
  TBCCR0 = 1;
  // Enable interrupts for TBCCR1-3, Set CCIE bit to 1 Which means interupt enabled CCIE is Capture/compare interupt enable
  TBCCTL0 = CCIE + ID_2;   

  // 16 bits, continueous mode, ACLK select: Start the timer
  TBCTL = TBSSEL_1 + CNTL_0 + MC_2 ;
  //
  LPM3;		
}
