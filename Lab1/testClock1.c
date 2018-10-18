#include<msp430.h>
#include<stdint.h>
#include<intrinsics.h>

#define GREEN 0x02 // Green Led 0000 0010

#define delay_100ms(x) \
do{ \
uint16_t i = (x);  \
while(i--) __delay_cycles(600000);  \
}while(0)

// Enable high speed source MCLK from XT2 (a crystal)
// It follows the procedure in 4-12 in MSP430 user guide.
void Switch_XT2()
{
	register uint8_t i; 
	do{
		//1. switch on the crystal oscillator
		BCSCTL1 &= ~XT2OFF;
		//2. clear OFIFG flag
		IFG1 &= ~OFIFG;
		//3. wait approximately 50 usec
		i=255;
		while(i--);
	// 4. test OFIFG bit and repeat steps 1-4 till OFIFG is cleared 
	}while(IFG1&OFIFG);
	//5. Select XT2
	BCSCTL2=SELM_2;
}
int main()
{
	WDTCTL = WDTPW + WDTHOLD;
	Switch_XT2();
	while(1)
	{
	  
	  // turn on the green led and wait for 4 seconds
	  P4DIR |= GREEN; // set the Pin 4 Dir to the output direction
	  P4OUT &= ~GREEN; // set the Output to low
	  delay_100ms(4); // wait for 400 milliseconds
	  
	  // Turn off the green led and wait for 4 seconds
	  P4DIR &= ~GREEN; // set the Pin 4 Dir to the input direction

	  
	  delay_100ms(1); // wait for 1 milliseconds

	  
		//__delay_cycles(6000000);
	}

	return 0;
}
