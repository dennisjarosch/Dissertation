/*
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

importScripts("lib/chunk_ab.js", "lib/complex.js", "lib/analysis.js");

// in samples
var nWindow = 256;
var overlap = 0;

function getFile(url) {
	var req = new XMLHttpRequest();
	req.open('GET', url, false);
	// try to get data as ArrayBuffer, or at least unparsed
	if (req.hasOwnProperty("responseType"))
		req.responseType="arraybuffer";
	else
		req.overrideMimeType('text/plain; charset=x-user-defined');

	req.send(null);

	return req.response;
}

function analyze(data) {
	var chunk  = new Chunk();

	chunk.read(data);

	var bytesPerWin = nWindow * chunk.fmt.blockAlign; 
	var incr 	    = (nWindow - overlap) * chunk.fmt.blockAlign;
	var iterations  = Math.floor((chunk.data.size - bytesPerWin) / incr) + 1;

	if ((chunk.data.size - bytesPerWin) % incr)
		++iterations;

	var bytesPerSample = chunk.fmt.bitsPerSample / 8;
	var samplingMask   = Math.pow(2, chunk.fmt.bitsPerSample) - 1;
	var twoCompMask    = 1 << (chunk.fmt.bitsPerSample - 1);

	// we support only 2 channels (stereo) for now
	leftChannel  = new Array(nWindow);
	rightChannel = new Array(nWindow);

	var left, right;
	var resultLeft, resultRight;
	var magsLeft, magsRight;
	var complex;
	var dataOffset = chunk.data.offset;

	if (chunk.fmt.bitsPerSample == 16)
	{
		var int16View = new Int16Array(data, chunk.data.offset);

		var getLeftData = function(arrayView, n) {
			return arrayView[2 * n];
		}

		var getRightData = function(arrayView, n) {
			return arrayView[2 * n + 1];
		}
	}

	for (var i = 0; i < iterations - 1; i++)
	{
		for (var n = 0; n < nWindow; n++)
		{
			left  = getLeftData(int16View, n);
			right = getRightData(int16View, n);
/*
			// remove unwanted bits, to support 16 and 24 bit sampling
			left  = left & samplingMask;
			right = right & samplingMask;

			// convert to 2 complement signed
			left  = left - ((left & twoCompMask) << 1);
			right = right - ((right & twoCompMask) << 1);
*/
			leftChannel[n]  = new Complex(0, left * hann(n, nWindow - 1));
			rightChannel[n] = new Complex(0, right * hann(n, nWindow - 1));
		}

		// Cooley-Tukey FFT
		resultLeft  = fft(leftChannel);
		resultRight = fft(rightChannel);

		magsLeft  = magnitude(resultLeft);	
		magsRight = magnitude(resultRight);	

		dataOffset += incr;
	}
}

self.addEventListener("message", function(event) {
	var data = getFile("Let Test1.wav");
	analyze(data);

	postMessage();
}, false);

