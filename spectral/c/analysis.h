/*
  Copyright (c) 2008 the authors listed in the page history of
  http://en.literateprograms.org/Cooley-Tukey_FFT_algorithm_(C)
  as of 25 Feb 2011.

  Modifications Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "complex.h"

double     hann(int n, int N);
complex_t* fft(complex_t* x, int N /* must be a power of 2 */);
double*    amplitude(complex_t *x, int N);

#endif
