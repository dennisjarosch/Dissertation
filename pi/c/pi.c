/*
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define POW_NEG1(x) (1 - ((((int)(x)) & 1) << 1)) 

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: pi <number of iterations>\n");
		exit(EXIT_FAILURE);
	}

	int 		n;
	long double pi = 0;
	long int 	iterations = atol(argv[1]);

	struct timeval 	tv_start, tv_end;
	struct timezone tz;

	gettimeofday(&tv_start, &tz);

	for (n = 0; n < iterations; n++)
		pi = pi + (double)POW_NEG1(n) / ((2 * n) + 1);

	pi = pi * 4;

	gettimeofday(&tv_end, &tz);

	unsigned duration = (tv_end.tv_sec * 1000 + tv_end.tv_usec / 1000) - (tv_start.tv_sec * 1000 + tv_start.tv_usec / 1000);

	printf("Pi is %1.32Lf, time was %u ms\n", pi, duration);
	exit(EXIT_SUCCESS);
}

