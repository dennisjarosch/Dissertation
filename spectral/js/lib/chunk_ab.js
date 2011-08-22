/*
  Parses a binary WAVE audio file using the ArrayBufferView interface.

  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

// what a hack: define static buffer size to speed up ArrayViews
var bufSize = 256;

Chunk = function() {
	this.fmt  = new Object;
	this.data = new Object;
}

function stringFrom8Array(uint8Array) {
	var stringArray = [];

	for (var i = 0; i < uint8Array.length; ++i)
		stringArray[i] = uint8Array[i];

	return String.fromCharCode.apply(null, stringArray);
}

Chunk.prototype.read = function(data) {
	var ui16View = new Uint16Array(data, 0, bufSize);
	var ui32View = new Uint32Array(data, 0, bufSize);

	// initial chunk
	this.id 	= stringFrom8Array(new Uint8Array(data, 0, 4));
    this.size   = ui32View[1];
    this.format = stringFrom8Array(new Uint8Array(data, 8, 4));

	// format chunk
	this.fmt.id					= stringFrom8Array(new Uint8Array(data, 12, 4));
    this.fmt.size               = ui32View[4];
    this.fmt.format             = ui16View[10];
    this.fmt.numChannels        = ui16View[11];
    this.fmt.sampleRate         = ui32View[6];
    this.fmt.byteRate           = ui32View[7];
    this.fmt.blockAlign         = ui16View[16];
    this.fmt.bitsPerSample      = ui16View[17];
    // extended
    this.fmt.extSize            = '';
    this.fmt.validBitsPerSample = '';
    this.fmt.channelMask        = '';
    this.fmt.guid 				= ''; 

	if (this.fmt.size > 16)
	{
		this.fmt.extSize = ui16View[18];
		if (this.fmt.extSize > 0)
		{
			this.fmt.validBitsPerSample = ui16View[19];
			this.fmt.channelMask        = ui32View[10];
			this.fmt.guid               = stringFrom8Array(new Uint8Array(data, 40, 16));
		}
	}

	var offset = 20 + this.fmt.size;
	ui32View = new Uint32Array(data, offset + 4, bufSize);
	ui16View = new Uint16Array(data, offset + 4, bufSize);

	// fact chunk
	var id = stringFrom8Array(new Uint8Array(data, offset, 4));
	if (id == "fact")
	{
		this.fact = new Object();

		this.fact.id 		   = id;
		this.fact.size 		   = ui32View[0];
		this.fact.sampleLength = ui32View[1];

		offset += 12;
	}

	ui32View = new Uint32Array(data, offset + 4, bufSize);

	// data
	this.data.id     = stringFrom8Array(new Uint8Array(data, offset, 4));
	this.data.size   = ui32View[0];
	this.data.offset = offset + 8;
}
