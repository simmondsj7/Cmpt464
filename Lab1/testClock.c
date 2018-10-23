#include<msp430.h>
#include<stdint.h>
#include<intrinsics.h>

#define G 0x02

#define wait_100ms(x)				\
  do{\
    for(uint8_t i = 0; i<x; i ++)\
      __delay_cycles(600000);
}while(0)

void switch_XT2()  {
   uint8_t i;

  do {
    BCSCTL1 &= ~XT2OFF;

    IFG1 &= ~OFIFG;

    i = 255;
    while(i--); //need to wait a certain amount of time going to need to do noOP
  } while(IFG1&0FIFG);

  BCSCTL2 = SELM_2;
}

void switch_LED(){
  while(1){
    // Turning Green on
    P4DIR |= G; //specify the direction
    P4OUT &= ~G;
    wait_100ms(10);

    //Turn Green off
    P4DIR &= ~G; // make the Direction 0
    wait_100ms(10);
  }
  
}


int main() {

  WDTCTL = WDTPW + WDTHOLD;
  switch_XT2();
  
  return 0;

}


