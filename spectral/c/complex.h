/*
  Copyright (c) 2008 the authors listed in the page history of
  http://en.literateprograms.org/Cooley-Tukey_FFT_algorithm_(C)
  as of 25 Feb 2011.

  Modifications Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct Complex 
{
	double re;
	double im;
} complex_t;

double complexMagnitude(complex_t c);
complex_t complexFromPolar(double r, double theta_radians);
complex_t complexAdd(complex_t left, complex_t right);
complex_t complexSub(complex_t left, complex_t right); 
complex_t complexMult(complex_t left, complex_t right);

#endif

