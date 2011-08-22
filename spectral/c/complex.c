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

