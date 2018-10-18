#define R 0x04 //define all LEDs..All for testing only need red
#define G 0x02
#define Y 0x08

#define PERIOD 32768 //Number of clock cycles in one second

#define DASH (uint_16)(PERIOD)*(0.360) //360ms for Dash Period
#define DOT 1 (uint_16)(PERIOD)*(0.120) // 120ms for Dot Period

#define LED_INIT \
do{ \
  P4DIR |= R+G+Y; \
  P4OUT |= R+G+Y; \
}while(0) //init all of the LEDs to be off

#define DOT_ON \
do{ \
  P4OUT &= ~R; \
} while(0) 

#define DASH_ON \
do{ \
  P4DIR &= ~R; \
} while(0)

#define LOW_POWER_MODE LPM3

//selecting peripheral mode
#define INIT \
do{ \
  P3SEL |= 0x30; \
}while(0)

struct Buffer{
  uint8_t size;
  uint8_t in;
  uint8_t out;
  uint8_t data[64];
};
