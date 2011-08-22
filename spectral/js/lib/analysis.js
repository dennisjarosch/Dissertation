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

const cRef = 0.00001;

function hann(n, N) {
	// according to http://www.mathworks.com/help/toolbox/signal/hann.html
	return 0.5 - 0.5 * Math.cos(2 * Math.PI * n / N);
}

function fft(x) {
	var N = x.length;
	var X = new Array(N);

	if (N == 1) 
	{
		X[0] = new Complex(x[0].im, x[0].re);
		return X;
	}

	var e = new Array(N / 2);
	var d = new Array(N / 2); 

	for (var k = 0; k < N / 2; k++) 
	{
		e[k] = x[2 * k];
		d[k] = x[2 * k + 1];
	}

	var E = fft(e);
	var D = fft(d);

	for (k = 0; k < N / 2; k++) 
	{
		/* Multiply entries of D by the twiddle factors e^(-2*pi*i/N * k) */
		c = complexFromPolar(1, -2.0 * Math.PI * k / N);
		D[k] = complexMult(c, D[k]);
	}

	for (k = 0; k < N / 2; k++) 
	{
		X[k] 		 = complexAdd(E[k], D[k]);
		X[k + N / 2] = complexSub(E[k], D[k]);
	}

	return X;
}

function amplitude(x) {
	var N = x.length;
	var result = new Array(N);

	/* convert magnitudes to true amplitudes and scale to decibels */

	result[0] = 20 * Math.log( (x[0].magnitude() / N) / cRef);

	for (var i = 1; i < N / 2; i++)	
		result[i] = 20 * Math.log( (2 * x[i].magnitude() / N) / cRef);	

	return result;	
}

function getLeftChannel16Bit(arrayView, n, dataOffset) {
	return arrayView[2 * n + dataOffset];
}

function getRightChannel16Bit(arrayView, n, dataOffset) {
	return arrayView[2 * n + 1 + dataOffset];
}

function getLeftChannel24Bit(arrayView, n, dataOffset) {
	var i = dataOffset + 6 * n;

	var left = arrayView[i] | 
               arrayView[i + 1] << 8 | 
               arrayView[i + 2] << 16;

	// remove unwanted bits, to support 16 and 24 bit sampling
	left  = left & arrayView.samplingMask;
	// convert to 2 complement signed
	left  = left - ((left & arrayView.twoCompMask) << 1);

	return left;
}

function getRightChannel24Bit(arrayView, n, dataOffset) {
	var i = dataOffset + 6 * n;

	var right = arrayView[i + 3] | 
		   	    arrayView[i + 4] << 8 |
		        arrayView[i + 5] << 16;

	right = right & arrayView.samplingMask;
	right = right - ((right & arrayView.twoCompMask) << 1);

	return right;
}		



