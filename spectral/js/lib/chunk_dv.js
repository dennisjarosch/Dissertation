/*
  Parses a binary WAVE audio file using the DataView interface.

  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

Chunk = function() {
	this.fmt  = new Object;
	this.data = new Object;
}

function getString(data, offset) {
	var stringArray = new Array(4);

	for (var i = 0; i < 4; i++)
		stringArray[i] = data.getUint8(offset + i);

	return String.fromCharCode.apply(null, stringArray);
}

Chunk.prototype.read = function(data) {
	// initial chunk
	this.id 	= getString(data, 0);
    this.size   = data.getUint32(4, true);
    this.format = getString(data, 8);

	// format chunk
	this.fmt.id					= getString(data, 12);
    this.fmt.size               = data.getUint32(16, true);
    this.fmt.format             = data.getUint16(20, true);
    this.fmt.numChannels        = data.getUint16(22, true);
    this.fmt.sampleRate         = data.getUint32(24, true);
    this.fmt.byteRate           = data.getUint32(28, true);
    this.fmt.blockAlign         = data.getUint16(32, true);
    this.fmt.bitsPerSample      = data.getUint16(34, true);
    // extended
    this.fmt.extSize            = '';
    this.fmt.validBitsPerSample = '';
    this.fmt.channelMask        = '';
    this.fmt.guid 				= ''; 

	if (this.fmt.size > 16)
	{
		this.fmt.extSize = data.getUint16(36, true);  
		if (this.fmt.extSize > 0)
		{
			this.fmt.validBitsPerSample = data.getUint16(38, true);
			this.fmt.channelMask        = data.getUint32(40, true);
			this.fmt.guid               = data.getUint8(44);
		}
	}

	var offset = 20 + this.fmt.size;

	// fact chunk
	var id = getString(data, offset);
	if (id == "fact")
	{
		this.fact = new Object();

		this.fact.id 		   = id;
		this.fact.size 		   = data.getUint32(offset + 4, true);
		this.fact.sampleLength = data.getUint32(offset + 8, true);

		offset += 12;
	}

	// data
	this.data.id     = getString(data, offset);
	this.data.size   = data.getUint32(offset + 4, true);
	this.data.offset = offset + 8;
}
