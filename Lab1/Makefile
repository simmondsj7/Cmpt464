CC=msp430-gcc
CFLAGS=-mmcu=msp430f1611 -std=c99 -Wall

Clock1:
	$(CC) $(CFLAGS) -o Clock1 testClock1.c

BuildTest:
	$(CC) $(CFLAGS) -o Image test.c

Clean:
	rm -f Image
