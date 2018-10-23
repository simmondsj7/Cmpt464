#define R 0x04 //define all LEDs..All for testing only need red
#define G 0x02
#define Y 0x08

#define s 26

#define PERIOD 32768 //Number of clock cycles in one second

#define DASH (uint_16)(PERIOD)*(0.360) //360ms for Dash Period
#define DOT 1 (uint_16)(PERIOD)*(0.120) // 120ms for Dot Period

//init all of the LEDs to be off
#define LED_INIT \
do{ \
  P4DIR |= R+G+Y; \
  P4OUT |= R+G+Y; \
}while(0) 


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

#define INIT_STRUCT \
do{ \
 morse[0].hex =0x61; \
 morse[0].code =".-"; \
} while(0)

#define SET_MAP \
do { \
 letters[0].letter ="a"; \
 letters[0].number = 0; \
} while(0)

// Macro Function that checks if char is one of the letters
// if it is than return the char* of the morse code value
#define check_character(char, morse_struct, letter_struct) \
do { \
 for(int i=0; i<= sizeof(MapAlpha(letter_struct)); i ++){ \
     if(MapAlpha(letter_struct[i].letter) = char(char)){ \
 	return MorseCode(morse_struct[MapAlpha(letter_struct[i].number)].code); \
	} else {\
	return NULL; \
	}; \
    }; \
} while(0)
 

struct Buffer{
  uint8_t size;
  uint8_t in;
  uint8_t out;
  char* data[64];
};

typedef struct{
    uint8_t hex; 
    char* code;
} MorseCode;

typedef struct{
  char* letter; 
  uint8_t number; 
} MapAlpha;

MorseCode morse[s];
MapAlpha letters[s];
	


