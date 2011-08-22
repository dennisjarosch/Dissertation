/* 
  Copyright (c) 2011 the authors listed at the following URL, and/or
  the authors of referenced articles or incorporated external code:
  http://en.literateprograms.org/Cooley-Tukey_FFT_algorithm_(C)?action=history&offset=20081117110818
 
  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:
 
  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  
  Retrieved from: http://en.literateprograms.org/Cooley-Tukey_FFT_algorithm_(C)?oldid=15458

  Modified to own needs, 
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
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
