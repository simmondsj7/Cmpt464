CC=msp430-gcc
CFLAGS=-mmcu=msp430f1611 -std=c99 -Wall

Image1:
	$(CC) $(CFLAGS) -o Image Excercise_#1.c
Image2:
	$(CC) $(CFLAGS) -o Image Excercise_#2.c
demo1:
	$(CC) $(CFLAGS) -o Image UART_demo1.c

demo2:
	$(CC) $(CFLAGS) -o Image UART_demo2.c

Clean:
	rm -f Image
