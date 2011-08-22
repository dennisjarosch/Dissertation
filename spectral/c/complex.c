/*
  Copyright (c) 2008 the authors listed in the page history of
  http://en.literateprograms.org/Cooley-Tukey_FFT_algorithm_(C)
  as of 25 Feb 2011.

  Modifications Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

#include "complex.h"
#include <math.h>

//_______________________________________________________________________

complex_t complexFromPolar(double r, double theta_radians) 
{
	complex_t result;
	result.re = r * cos(theta_radians);
	result.im = r * sin(theta_radians);

	return result;
}
//_______________________________________________________________________

double complexMagnitude(complex_t c) 
{
	return sqrt(c.re * c.re + c.im * c.im);
}
//_______________________________________________________________________

complex_t complexAdd(complex_t left, complex_t right) 
{
	complex_t result;
	result.re = left.re + right.re;
	result.im = left.im + right.im;
	
	return result;
}
//_______________________________________________________________________

complex_t complexSub(complex_t left, complex_t right) 
{
	complex_t result;
	result.re = left.re - right.re;
	result.im = left.im - right.im;
    
	return result;
}
//_______________________________________________________________________

complex_t complexMult(complex_t left, complex_t right) 
{
	complex_t result;
	result.re = left.re * right.re - left.im * right.im;
	result.im = left.re * right.im + left.im * right.re;
    
	return result;
}
//_______________________________________________________________________

