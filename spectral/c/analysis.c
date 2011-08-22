/*
  Copyright (c) 2008 the authors listed in the page history of
  http://en.literateprograms.org/Cooley-Tukey_FFT_algorithm_(C)
  as of 25 Feb 2011.

  Modifications Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

#include "analysis.h"
#include <math.h>
#include <stdlib.h>

#define A_REF 0.00001
//_______________________________________________________________________

double hann(int n, int N)
{
	/* according to http://www.mathworks.com/help/toolbox/signal/hann.html */
	return 0.5 - 0.5 * cos(2 * M_PI * n / N);
}
//______________________________________________________________________

complex_t* fft(complex_t* x, int N) 
{
	complex_t* X = (complex_t*) malloc(sizeof(struct Complex) * N);
	complex_t * d, * e, * D, * E;
	int k;

	if (N == 1) 
	{
		X[0] = x[0];
		return X;
	}

	e = (complex_t*) malloc(sizeof(struct Complex) * N / 2);
	d = (complex_t*) malloc(sizeof(struct Complex) * N / 2);

	for (k = 0; k < N / 2; k++) 
	{
		e[k] = x[2 * k];
		d[k] = x[2 * k + 1];
	}

	E = fft(e, N / 2);
	D = fft(d, N / 2);

	free(e);
	free(d);

	for (k = 0; k < N / 2; k++) 
	{
		/* Multiply entries of D by the twiddle factors e^(-2*pi*i/N * k) */
		D[k] = complexMult(complexFromPolar(1, -2.0 * M_PI *k / N), D[k]);
	}

	for (k = 0; k < N / 2; k++) 
	{
		X[k]       = complexAdd(E[k], D[k]);
		X[k + N/2] = complexSub(E[k], D[k]);
	}

	free(D);
	free(E);
	return X;
}
//______________________________________________________________________

double* amplitude(complex_t *x, int N)
{
	double *result = malloc(sizeof(double) * N / 2);

	/* convert magnitudes to true amplitudes and scale to decibels */
	result[0] = 20 * log((complexMagnitude(x[0]) / N) / A_REF);

	int i;
	for (i = 1; i < N / 2; i++)	
		result[i] = 20 * log((2 * complexMagnitude(x[i]) / N) / A_REF);	

	return result;	
}
//______________________________________________________________________
