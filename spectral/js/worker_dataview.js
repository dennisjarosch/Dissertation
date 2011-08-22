/*
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

importScripts("lib/chunk_dv.js", "lib/complex.js", "lib/analysis.js");

// in samples
var nWindow = 256;
var overlap = 0;

function getFile(url) {
	var moz = false;

	var req = new XMLHttpRequest();
	req.open('GET', url, false);
	// try to get data as ArrayBuffer, or at least unparsed
	if (req.hasOwnProperty("responseType"))
		req.responseType="arraybuffer";
//			else if (req.hasOwnProperty("mozResponseArrayBuffer"))
	else if (req.mozResponseArrayBuffer == null)
		moz = true;
	else
		throw "Aborting, ArrayBuffers are not supported."

	req.send(null);

	if (moz)
		return req.mozResponseArrayBuffer;

	return req.response;
}

function analyze(data) {
	var data  = new DataView(file);
	var chunk = new Chunk();

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

	for (var i = 0; i < iterations - 1; i++)
	{
		for (var n = 0; n < nWindow; n++)
		{
			if ( (dataOffset + chunk.fmt.blockAlign * n) < chunk.data.size + chunk.data.offset)
			{
				left  = data.getInt32(dataOffset + (chunk.fmt.blockAlign * n), true);
				right = data.getInt32(dataOffset + (chunk.fmt.blockAlign * n) + bytesPerSample, true);

				// remove unwanted bits, to support 16 and 24 bit sampling
				left  = left & samplingMask;
				right = right & samplingMask;

				// convert to 2 complement signed
				left  = left - ((left & twoCompMask) << 1);
				right = right - ((right & twoCompMask) << 1);
			}
			else
			{
				left  = 0;
				right = 0;	
			}

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
	var data = getFile(event.data);
	analyze(data);

	postMessage();
}, false);

