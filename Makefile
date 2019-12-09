CC = gcc
CFLAGS = -Wall -std=c11

all:pa2

pa2:
	$(CC) $(CFLAGS) pa2.c -o pa2 -lgsl -lgslcblas -lm 

clean:
	rm -f pa2
