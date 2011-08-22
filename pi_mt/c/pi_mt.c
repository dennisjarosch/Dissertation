/*
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

#define POW_NEG1(x) (1 - ((((int)(x)) & 1) << 1)) 
#define NUM_THREADS 2

typedef struct arg
{
	long int from;
	long int to;
} arg_t;

long double 	pi_x = 0.0;
pthread_mutex_t lock_x;

//_______________________________________________________________________

void* crunch(void *ptr)
{
	int n;
	long double pi = 0.0;

	arg_t *args = (arg_t*)ptr;

	/* compute pi */
	for (n = args->from; n < args->to; n++)
		pi = pi + (double)POW_NEG1(n) / ((2 * n) + 1);

	/* and store to the shared pi_x variable */
	pthread_mutex_lock(&lock_x);
	pi_x = pi_x + pi;
	pthread_mutex_unlock(&lock_x);

	return NULL;
}

//_______________________________________________________________________

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: pi <number of iterations>\n");
		exit(EXIT_FAILURE);
	}

	int 	 	i;
	long int 	iterations = atol(argv[1]) / NUM_THREADS;

	pthread_t thread[NUM_THREADS];
	arg_t 	  args[NUM_THREADS];

	struct timeval 	tv_start, tv_end;
	struct timezone tz;

	gettimeofday(&tv_start, &tz);

	/* launch worker threads */
	for (i = 0; i < NUM_THREADS; i++)
	{
		args[i].from = i * iterations;
		args[i].to	 = args[i].from + iterations;

		pthread_create( &thread[i], NULL, crunch, (void*) &args[i]);
	}

	/* wait for their completion */
	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(thread[i], NULL);

	pi_x = pi_x * 4;

	gettimeofday(&tv_end, &tz);

	unsigned duration = (tv_end.tv_sec * 1000 + tv_end.tv_usec / 1000) - (tv_start.tv_sec * 1000 + tv_start.tv_usec / 1000);

	printf("Pi is %1.32Lf, time was %u ms\n", pi_x, duration);

	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
}

