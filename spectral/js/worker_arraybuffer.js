/*
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

importScripts("lib/chunk_ab.js", "lib/complex.js", "lib/analysis.js");

// in samples
var nWindow = 256;
var overlap = 128;

function getFile(url) {
	var moz = false;

	var req = new XMLHttpRequest();
	req.open('GET', url, false);
	// try to get data as ArrayBuffer, or at least unparsed
	if (req.hasOwnProperty("responseType"))
		req.responseType="arraybuffer";
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
	var chunk = new Chunk();

	chunk.read(data);

	var bytesPerWin = nWindow * chunk.fmt.blockAlign; 
	var incr 	    = (nWindow - overlap) * chunk.fmt.blockAlign;
	var iterations  = Math.floor((chunk.data.size - bytesPerWin) / incr) + 1;

	if ((chunk.data.size - bytesPerWin) % incr)
		++iterations;

	var bytesPerSample = chunk.fmt.bitsPerSample / 8;

	// we support only 2 channels (stereo) for now
	leftChannel  = new Array(nWindow);
	rightChannel = new Array(nWindow);

	var left, right;
	var resultLeft, resultRight;
	var ampsLeft, ampsRight;
	var complex;
	var dataOffset = 0; //chunk.data.offset;

	if (chunk.fmt.bitsPerSample == 16)
	{
		var dataView = new Int16Array(data, chunk.data.offset);

		var getLeftData  = getLeftChannel16Bit;
		var getRightData = getRightChannel16Bit;
	}
	else if (chunk.fmt.bitsPerSample == 24)
	{
		var dataView = new Uint8Array(data, chunk.data.offset);
		dataView.samplingMask = Math.pow(2, chunk.fmt.bitsPerSample) - 1;
		dataView.twoCompMask  = 1 << (chunk.fmt.bitsPerSample - 1);

		var getLeftData  = getLeftChannel24Bit;
		var getRightData = getRightChannel24Bit;
	}

	for (var i = 0; i < iterations - 1; i++)
	{
		for (var n = 0; n < nWindow; n++)
		{
			left  = getLeftData(dataView, n, dataOffset);
			right = getRightData(dataView, n, dataOffset);			

			leftChannel[n]  = new Complex(0, left * hann(n, nWindow - 1));
			rightChannel[n] = new Complex(0, right * hann(n, nWindow - 1));
		}

		// Cooley-Tukey FFT
		resultLeft  = fft(leftChannel);
		resultRight = fft(rightChannel);

		ampsLeft  = amplitude(resultLeft);	
		ampsRight = amplitude(resultRight);	

		dataOffset += incr;
	}
}

self.addEventListener("message", function(event) {
	var data = getFile(event.data);
	analyze(data);

	postMessage();
}, false);

