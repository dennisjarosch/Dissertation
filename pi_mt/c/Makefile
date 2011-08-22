# Makefile
CC = gcc
CFLAGS =  -Include -c -Wall -Wno-long-long -Werror -DXP_UNIX -O2 -m64
LDFLAGS = -lm -lpthread

.PHONY: all clean

all: pi_mt

pi_mt: pi_mt.o
	$(CC) -o pi_mt pi_mt.o $(LDFLAGS)

pi_mt.o: pi_mt.c
	$(CC) $(CFLAGS) pi_mt.c

clean:
	\rm -f pi_mt *.o *~
