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
  P4OUT &= ~R; \
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

MorseCode m[40] = {{.letter = 'a', .code = ".-", .length = 2},  // 0
{.letter = 'b', .code = "-...", .length = 4},                   // 1
{.letter = 'c', .code = "-.-.", .length = 4},                   // 2
{.letter = 'd', .code = "-..", .length = 3},                    // 3
{.letter = 'e', .code = ".", .length = 1},                      // 4
{.letter = 'f', .code = "..-.", .length = 4},                   // 5
{.letter = 'g', .code = "--.", .length = 3},                    // 6
{.letter = 'h', .code = "....", .length = 4},                   // 7
{.letter = 'i', .code = "..", .length = 2},                     // 8
{.letter = 'j', .code = ".---", .length = 4},                   // 9
{.letter = 'k', .code = "-.-", .length = 3},                    // 10
{.letter = 'l', .code = ".-..", .length = 4},                   // 11
{.letter = 'm', .code = "--", .length = 2},                     // 12
{.letter = 'n', .code = "-.", .length = 2},                     // 13
{.letter = 'o', .code = "---", .length = 3},                    // 14
{.letter = 'p', .code = ".--.", .length = 4},                   // 15
{.letter = 'q', .code = "--.-", .length = 4},                   // 16
{.letter = 'r', .code = ".-.", .length = 3},                    // 17
{.letter = 's', .code = "...", .length = 3},                    // 18
{.letter = 't', .code = "-", .length = 1},                      // 19
{.letter = 'u', .code = "..-", .length = 3},                    // 20
{.letter = 'v', .code = "...-", .length = 4},                   // 21
{.letter = 'w', .code = ".--", .length = 3},                    // 22
{.letter = 'x', .code = "-..-", .length = 4},                   // 23
{.letter = 'y', .code = "-.--", .length = 4},                   // 24
{.letter = 'z', .code = "--..", .length = 4},                   // 25
{.letter = '1', .code = ".----", .length = 5},                  // 26
{.letter = '2', .code = "..---", .length = 5},                  // 27
{.letter = '3', .code = "...--", .length = 5},                  // 28
{.letter = '4', .code = "....-", .length = 5},                  // 29
{.letter = '5', .code = ".....", .length = 5},                  // 30
{.letter = '6', .code = "-....", .length = 5},                  // 31
{.letter = '7', .code = "--...", .length = 5},                  // 32
{.letter = '8', .code = "---..", .length = 5},                  // 33
{.letter = '9', .code = "----.", .length = 5},                  // 34
{.letter = '0', .code = "-----", .length = 5},                  // 35
{.letter = '.', .code = ".-.-.-", .length = 6},                 // 36 - Full stop
{.letter = '@', .code = "...-.-", .length = 6},                 // 37 - End of Work
{.letter = '~', .code = "-.-.-", .length = 3}};                 // 38 - Starting Signal

MorseCode morse[s];
Buffer ringbuf= {.in=0, .out=0, .size=0};
