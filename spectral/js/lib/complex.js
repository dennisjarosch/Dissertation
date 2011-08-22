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

  Ported to JavaScript and modified to own needs, 
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
*/

Complex = function() {
	this.im = 0;
	this.re = 0;
}

Complex = function(im, re) {
	this.im = im;
	this.re = re;
}

Complex.prototype.magnitude = function() {
	return Math.sqrt(this.re * this.re + this.im * this.im);
}

function complexFromPolar(r, thetaRadians) {
	var re = r * Math.cos(thetaRadians);
	var im = r * Math.sin(thetaRadians);

	return new Complex(im, re);
}

function complexAdd(left, right) {
	return new Complex(left.im + right.im, left.re + right.re);
}

function complexSub(left, right) {
	return new Complex(left.im - right.im, left.re - right.re);
}

function complexMult(left, right) {
	var c = new Complex;

	c.re = left.re * right.re - left.im * right.im;
	c.im = left.re * right.im + left.im * right.re;

	return c;
}

