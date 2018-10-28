#include <msp430.h>
#include <stdint.h>
#include <string.h>

#define PERIOD 4000
// Define leds
#define RED_LED 0x04
#define GREEN_LED 0x02
#define YELLOW_LED 0x08
// Initialize leds
#define PIN_INIT do{	\
P4DIR |= (RED_LED+GREEN_LED+YELLOW_LED);	\
P4SEL &= ~(RED_LED+GREEN_LED+YELLOW_LED);	\
P4OUT |= (RED_LED+GREEN_LED+YELLOW_LED);	\
}while(0)

const char mesg[] = "Hello  ";
int index = 0;
// Turn ON timer
#define TIMER_ON do{ \
TBCTL = TBCLR; /*This resets the timer after turning it on*/\
TBCCR0 = 1; \
TBCCTL0 =  CCIE;	\
TBCTL = TBSSEL_1 + MC_2;		\
}while(0)

// NEXT Timer
#define TIMER_NEXT(x) do{ \
TBCCR0 += (x); \
}while(0)

//Turn OFF timer
#define TIMER_OFF do{ \
TBCCTL0 &=  ~CCIE; \
TBCTL = MC_0; /*Shut down the timer */ \
\
}while(0)

/////////////////////////////////////////////////////////////
/* Functions */
// The following function enable receiving from UART and send
void enable_uart_receive()
{
    /* pins for USART0/UART mode:
       P3.4: transmit data out
       P3.5: transmit data in  */
    P3SEL |= 0x30;		/* peripheral mode */

    /* USART initialization/reset procedure from UG 13.2.1 (pg. 13-4):*/
    /* Step #1: set SWRST in U0CTL */
    U0CTL = SWRST;

    /* Step #2: initialize all registers */
    U0CTL |= CHAR;		/* 8 in 8-N-1 */
    U0TCTL |= I2CSSEL_1;	/* ACLK */

    U0BR1 = 0;			/* configure baud rate; UG (pg. 13-16) */
    U0BR0 = 0x0D;		/* 2400 baud */
    U0MCTL = 0x6B;

    /* Step #3: enable USART via ME1 */
    ME1 |= UTXE0 | URXE0;	/* USART0 TX/RX enable */

    /* Step #4: clear USART software reset (SWRST) via software */
    U0CTL &= ~SWRST;

    /* Step #5: enable interrupts (optional) */
    IE1 |= URXIE0;		/* enable UART receive interrupts */

}
/////////////////////////////////////////////////////////////
/* Interrupt Handlers */
__attribute__((interrupt(USART0RX_VECTOR)))
void uart_rx_isr()
{
	char ch = U0RXBUF;
	switch(ch)
	{
		case 'y':
		index = 0;
		TIMER_ON;
		break;
		case 'n':
		TIMER_OFF;
		index=0;
		break;
	}
}
/* Timer Interrupt */
void __attribute__((interrupt (TIMERB0_VECTOR)))
timer_handler()
{
	P4OUT ^= RED_LED;
	IE1 |= UTXIE0;
	TIMER_NEXT(PERIOD);
}
/* Ready to send on the UART */
__attribute__((interrupt(USART0TX_VECTOR)))
void uart_tx_isr()
{
	U0TXBUF = mesg[index];
	index = (index+1)%strlen(mesg);
	IE1 &= ~UTXIE0;
}
/////////////////////////////////////////////////////////////
/* Main function */

int main() {
    WDTCTL = WDTPW | WDTHOLD;
    __eint();
    
    // Initialize output and Enable the UART to receive
    PIN_INIT;

    enable_uart_receive();
 

   // Go to low power mode
    LPM3;
}