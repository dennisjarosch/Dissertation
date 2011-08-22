# Makefile
CC = gcc
CFLAGS =  -Include -c -Wall -Wno-long-long -Werror -DXP_UNIX -O2 -m64
LDFLAGS = -lm

.PHONY: all clean

all: pi

pi: pi.o
	$(CC) -o pi pi.o $(LDFLAGS)

pi.o: pi.c
	$(CC) $(CFLAGS) pi.c

clean:
	\rm -f pi *.o *~
