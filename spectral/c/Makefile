# Makefile
CC = gcc
CFLAGS =  -Include -c -Wall -Werror -O2 -m64
LDFLAGS = -lm

.PHONY: all clean

all: spectral

spectral: spectral.o analysis.o complex.o
	$(CC) -o spectral spectral.o analysis.o complex.o $(LDFLAGS)

spectral.o: spectral.c analysis.c complex.c
	$(CC) $(CFLAGS) spectral.c analysis.c complex.c

clean:
	\rm -f spectral *.o *~
