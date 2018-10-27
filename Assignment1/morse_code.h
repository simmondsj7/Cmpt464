#define R 0x04 //define all LEDs..All for testing only need red
#define G 0x02
#define Y 0x08

#define s 26

#define PERIOD 32768 //Number of clock cycles in one second

#define DASH (uint16_t)(PERIOD)*(0.360) //360ms for Dash Period
#define DOT (uint16_t)(PERIOD)*(0.120) // 120ms for Dot Period

//init all of the LEDs to be off
#define LED_INIT \
do{ \
  P4DIR |= R+G+Y; \
  P4OUT |= R+G+Y; \
}while(0) 

#define SET_TIME(x) \
do{ \
  retval = PERIOD * x; \
} while(0)

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

typedef struct{
  uint8_t size;
  uint8_t in;
  uint8_t out;
  uint8_t data[64];
} Buffer;

typedef struct{
    char letter; 
    char code[7];
    int length;
} MorseCode;

MorseCode m[38] = {{.letter = 'a', .code = ".-", .length = 2},  
{.letter = 'b', .code = "-...", .length = 4}, 
{.letter = 'c', .code = "-.-.", .length}, {.letter = 'd', .code = "-..", .length = 3}};

MorseCode morse[s];
Buffer ringbuf= {.in=0, .out=0, .size=0};
