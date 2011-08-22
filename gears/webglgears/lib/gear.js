/*
  Copyright (c) 2011 Dennis Jarosch. All rights reserved.
  Use of this source code is governed by a BSD-style license that can be
  found in the LICENSE file.
*/

Buffer = function() {
	this.vertexObject;
	this.normalObject;
	this.colorObject;
	this.indexObject;
}


Gear = function(gl, innerRadius, outerRadius, width, teeth, toothDepth, color) {
	this.innerRadius	= innerRadius;
	this.outerRadius	= outerRadius;
	this.width			= width;
	this.teeth			= teeth;
	this.toothDepth		= toothDepth;
	this.color			= color;

	this.frontFace 		= new Buffer();
	this.frontTeeth 	= new Buffer();
	this.backFace 		= new Buffer();
	this.backTeeth  	= new Buffer();
	this.outwardTeeth	= new Buffer();
	this.cylinder		= new Buffer();

//	this.create(gl);
	this.createAll(gl);
}

/* create model */
/*
Gear.prototype.createFrontFace = function(gl, r0, r1) {
	var angle;
	var da = 2.0 * Math.PI / this.teeth / 4.0;

	var geometryData = [ ];
    var normalData 	 = [ ];
	var indexData	 = [ ];

   // GL_QUAD_STRIP
	for (var i = 0; i <= this.teeth; i++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;
		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle)); 
		geometryData.push(this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);

		if (i < this.teeth) 
		{
			geometryData.push(r0 * Math.cos(angle));
			geometryData.push(r0 * Math.sin(angle));
			geometryData.push(this.width * 0.5);

			geometryData.push(r1 * Math.cos(angle + 3 * da));
			geometryData.push(r1 * Math.sin(angle + 3 * da));
			geometryData.push(this.width * 0.5);

			normalData.push(0.0, 0.0, 1.0);
			normalData.push(0.0, 0.0, 1.0);
		}
	}

	// vertex buffer
	this.frontFace.vertexObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontFace.vertexObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(geometryData), gl.STATIC_DRAW);
	this.frontFace.vertexObject.itemSize = 3;
    this.frontFace.vertexObject.numItems = geometryData.length / this.frontFace.vertexObject.itemSize;

	// normal buffer
	this.frontFace.normalObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontFace.normalObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normalData), gl.STATIC_DRAW);
	this.frontFace.normalObject.itemSize = 3;
	this.frontFace.normalObject.numItems = normalData.length / this.frontFace.normalObject.itemSize;

	// color buffer - same color for all vertices
	var colors = [];
	
	for (var i=0; i<this.frontFace.vertexObject.numItems; i++)
		colors = colors.concat(this.color);

	this.frontFace.colorObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontFace.colorObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
    this.frontFace.colorObject.itemSize = 4;
    this.frontFace.colorObject.numItems = this.frontFace.vertexObject.numItems;
}


Gear.prototype.createBackFace = function(gl, r0, r1) {
	var angle;
	var da = 2.0 * Math.PI / this.teeth / 4.0;

	var geometryData = [ ];
    var normalData 	 = [ ];
	var indexData	 = [ ];

   // GL_QUAD_STRIP
	for (var i = 0; i <= this.teeth; i++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);

		if (i < this.teeth) 
		{
			geometryData.push(r1 * Math.cos(angle + 3 * da));
			geometryData.push(r1 * Math.sin(angle + 3 * da));
			geometryData.push(-this.width * 0.5);

			geometryData.push(r0 * Math.cos(angle));
			geometryData.push(r0 * Math.sin(angle));
			geometryData.push(-this.width * 0.5);

			normalData.push(0.0, 0.0, -1.0);
			normalData.push(0.0, 0.0, -1.0);
		}
	}

	// vertex buffer
	this.backFace.vertexObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.backFace.vertexObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(geometryData), gl.STATIC_DRAW);
	this.backFace.vertexObject.itemSize = 3;
    this.backFace.vertexObject.numItems = geometryData.length / this.backFace.vertexObject.itemSize;

	// normal buffer
	this.backFace.normalObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.backFace.normalObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normalData), gl.STATIC_DRAW);
	this.backFace.normalObject.itemSize = 3;
	this.backFace.normalObject.numItems = normalData.length / this.backFace.normalObject.itemSize;

	// color buffer - same color for all vertices
	var colors = [];
	
	for (var i=0; i<this.backFace.vertexObject.numItems; i++)
		colors = colors.concat(this.color);

	this.backFace.colorObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.backFace.colorObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
    this.backFace.colorObject.itemSize = 4;
    this.backFace.colorObject.numItems = this.backFace.vertexObject.numItems;
}


Gear.prototype.createFrontTeeth = function(gl, r1, r2) {
	var angle;
	var da = 2.0 * Math.PI / this.teeth / 4.0;

	var geometryData = [ ];
    var normalData 	 = [ ];
	var indexData	 = [ ];

	// GL_QUADS
	for (var i = 0; i < this.teeth; i++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + da));
		geometryData.push(r2 * Math.sin(angle + da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + 2 * da));
		geometryData.push(r2 * Math.sin(angle + 2 * da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle + 3 * da));
		geometryData.push(r1 * Math.sin(angle + 3 * da));
		geometryData.push(this.width * 0.5);

		indexData.push(4 * i, 4 * i + 1, 4 * i + 2);
		indexData.push(4 * i, 4 * i + 2, 4 * i + 3);

		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);
	}	

	// vertex buffer
	this.frontTeeth.vertexObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontTeeth.vertexObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(geometryData), gl.STATIC_DRAW);
	this.frontTeeth.vertexObject.itemSize = 3;
    this.frontTeeth.vertexObject.numItems = geometryData.length / this.frontTeeth.vertexObject.itemSize;

	// normal buffer
	this.frontTeeth.normalObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontTeeth.normalObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normalData), gl.STATIC_DRAW);
	this.frontTeeth.normalObject.itemSize = 3;
	this.frontTeeth.normalObject.numItems = normalData.length / this.frontTeeth.normalObject.itemSize;

	// color buffer - same color for all vertices
	var colors = [];
	
	for (var i=0; i<this.frontTeeth.vertexObject.numItems; i++)
		colors = colors.concat(this.color);

	this.frontTeeth.colorObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontTeeth.colorObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
    this.frontTeeth.colorObject.itemSize = 4;
    this.frontTeeth.colorObject.numItems = this.frontTeeth.vertexObject.numItems;

	// index buffer
	this.frontTeeth.indexObject = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.frontTeeth.indexObject);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indexData), gl.STATIC_DRAW);
	this.frontTeeth.indexObject.itemSize = 1;
	this.frontTeeth.indexObject.numItems = indexData.length;
}


Gear.prototype.createBackTeeth = function(gl, r1, r2) {
	var angle;
	var da = 2.0 * Math.PI / this.teeth / 4.0;

	var geometryData = [ ];
    var normalData 	 = [ ];
	var indexData	 = [ ];

	// GL_QUADS
	for (var i = 0; i < this.teeth; i++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;

		geometryData.push(r1 * Math.cos(angle + 3 * da));
		geometryData.push(r1 * Math.sin(angle + 3 * da));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + 2 * da));
		geometryData.push(r2 * Math.sin(angle + 2 * da));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + da));
		geometryData.push(r2 * Math.sin(angle + da));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		indexData.push(4 * i, 4 * i + 1, 4 * i + 2);
		indexData.push(4 * i, 4 * i + 2, 4 * i + 3);

		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);
	}

	// vertex buffer
	this.backTeeth.vertexObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.backTeeth.vertexObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(geometryData), gl.STATIC_DRAW);
	this.backTeeth.vertexObject.itemSize = 3;
    this.backTeeth.vertexObject.numItems = geometryData.length / this.backTeeth.vertexObject.itemSize;

	// normal buffer
	this.backTeeth.normalObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.backTeeth.normalObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normalData), gl.STATIC_DRAW);
	this.backTeeth.normalObject.itemSize = 3;
	this.backTeeth.normalObject.numItems = normalData.length / this.backTeeth.normalObject.itemSize;

	// color buffer - same color for all vertices
	var colors = [];
	
	for (var i=0; i<this.backTeeth.vertexObject.numItems; i++)
		colors = colors.concat(this.color);

	this.backTeeth.colorObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.backTeeth.colorObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
    this.backTeeth.colorObject.itemSize = 4;
    this.backTeeth.colorObject.numItems = this.backTeeth.vertexObject.numItems;

	// index buffer
	this.backTeeth.indexObject = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.backTeeth.indexObject);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indexData), gl.STATIC_DRAW);
	this.backTeeth.indexObject.itemSize = 1;
	this.backTeeth.indexObject.numItems = indexData.length;

}


Gear.prototype.createOutwardTeeth = function(gl, r1, r2) {
	var angle, u, v, len;
	var da = 2.0 * Math.PI / this.teeth / 4.0;

	var geometryData = [ ];
    var normalData 	 = [ ];
	var indexData	 = [ ];

	// GL_QUAD_STRIP
	for (var i = 0; i < this.teeth; i++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		u = r2 * Math.cos(angle + da) - r1 * Math.cos(angle);
		v = r2 * Math.sin(angle + da) - r1 * Math.sin(angle);
		len = Math.sqrt(u * u + v * v);
		u /= len;
		v /= len;

		normalData.push(v, -u, -0.0);
		normalData.push(v, -u, -0.0);

		geometryData.push(r2 * Math.cos(angle + da));
		geometryData.push(r2 * Math.sin(angle + da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + da));
		geometryData.push(r2 * Math.sin(angle + da));
		geometryData.push(-this.width * 0.5);

		normalData.push(Math.cos(angle), Math.sin(angle), 0.0);
		normalData.push(Math.cos(angle), Math.sin(angle), 0.0);

		geometryData.push(r2 * Math.cos(angle + 2 * da));
		geometryData.push(r2 * Math.sin(angle + 2 * da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + 2 * da));
		geometryData.push(r2 * Math.sin(angle + 2 * da));
		geometryData.push(-this.width * 0.5);

		u = r1 * Math.cos(angle + 3 * da) - r2 * Math.cos(angle + 2 * da);
		v = r1 * Math.sin(angle + 3 * da) - r2 * Math.sin(angle + 2 * da);

		normalData.push(v, -u, 0.0);
		normalData.push(v, -u, 0.0);

		geometryData.push(r1 * Math.cos(angle + 3 * da));
		geometryData.push(r1 * Math.sin(angle + 3 * da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle + 3 * da));
		geometryData.push(r1 * Math.sin(angle + 3 * da));
		geometryData.push(-this.width * 0.5);

		normalData.push(Math.cos(angle), Math.sin(angle), 0.0);
		normalData.push(Math.cos(angle), Math.sin(angle), 0.0);
	}

	// vertex buffer
	this.outwardTeeth.vertexObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.outwardTeeth.vertexObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(geometryData), gl.STATIC_DRAW);
	this.outwardTeeth.vertexObject.itemSize = 3;
    this.outwardTeeth.vertexObject.numItems = geometryData.length / this.outwardTeeth.vertexObject.itemSize;

	// normal buffer
	this.outwardTeeth.normalObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.outwardTeeth.normalObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normalData), gl.STATIC_DRAW);
	this.outwardTeeth.normalObject.itemSize = 3;
	this.outwardTeeth.normalObject.numItems = normalData.length / this.outwardTeeth.normalObject.itemSize;

	// color buffer - same color for all vertices
	var colors = [];
	
	for (var i=0; i<this.outwardTeeth.vertexObject.numItems; i++)
		colors = colors.concat(this.color);

	this.outwardTeeth.colorObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.outwardTeeth.colorObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
    this.outwardTeeth.colorObject.itemSize = 4;
    this.outwardTeeth.colorObject.numItems = this.outwardTeeth.vertexObject.numItems;
}


Gear.prototype.createCylinder = function(gl, r0) {
	var angle;

	var geometryData = [ ];
    var normalData 	 = [ ];
	var indexData	 = [ ];
	
	// GL_QUAD_STRIP
	for (var i = 0; i <= this.teeth; i++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;
		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);
		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		// normals
		normalData.push(-Math.cos(angle));
		normalData.push(-Math.sin(angle));
		normalData.push(0);
		normalData.push(-Math.cos(angle));
		normalData.push(-Math.sin(angle));
		normalData.push(0);
	}	

	// vertex buffer
	this.cylinder.vertexObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.cylinder.vertexObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(geometryData), gl.STATIC_DRAW);
	this.cylinder.vertexObject.itemSize = 3;
    this.cylinder.vertexObject.numItems = geometryData.length / this.cylinder.vertexObject.itemSize;

	// normal buffer
	this.cylinder.normalObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.cylinder.normalObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normalData), gl.STATIC_DRAW);
	this.cylinder.normalObject.itemSize = 3;
	this.cylinder.normalObject.numItems = normalData.length / this.cylinder.normalObject.itemSize;

	// color buffer - same color for all vertices
	var colors = [];
	
	for (var i=0; i<this.cylinder.vertexObject.numItems; i++)
		colors = colors.concat(this.color);

	this.cylinder.colorObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.cylinder.colorObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
    this.cylinder.colorObject.itemSize = 4;
    this.cylinder.colorObject.numItems = this.cylinder.vertexObject.numItems;
}


Gear.prototype.create = function(gl) {
	var geometryData = [ ];
    var normalData 	 = [ ];
	var indexData	 = [ ];

	var r0 = this.innerRadius;
	var r1 = this.outerRadius - this.toothDepth / 2.0;
	var r2 = this.outerRadius + this.toothDepth / 2.0;

	this.createFrontFace(gl, r0, r1);
	this.createFrontTeeth(gl, r1, r2);
	this.createBackFace(gl, r0, r1);
	this.createBackTeeth(gl, r1, r2);
	this.createOutwardTeeth(gl, r1, r2);
	this.createCylinder(gl, r0);
}
*/

Gear.prototype.createAll = function(gl)
{
	var geometryData = [ ];
    var normalData 	 = [ ];
	var indexData	 = [ ];

	var angle, u, v, len, j = 0;

	var r0 = this.innerRadius;
	var r1 = this.outerRadius - this.toothDepth / 2.0;
	var r2 = this.outerRadius + this.toothDepth / 2.0;
	// increase outer radius to resolve drawing errors for front and back faces
	var r1c = r1 + 0.05;

	var da = 2.0 * Math.PI / this.teeth / 4.0;

	// draw front face
	for (i = 0; i <= this.teeth; i+=2, j++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;
		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle)); 
		geometryData.push(this.width * 0.5);

		geometryData.push(r1c * Math.cos(angle));
		geometryData.push(r1c * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);

		angle = (i + 1) * 2.0 * Math.PI / this.teeth;
		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle)); 
		geometryData.push(this.width * 0.5);

		geometryData.push(r1c * Math.cos(angle));
		geometryData.push(r1c * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);

		// [0, 2, 3] and [0, 1, 3]
		indexData.push(4 * j, 4 * j + 2, 4 * j + 3);
		indexData.push(4 * j, 4 * j + 1, 4 * j + 3);
		
		if (i < this.teeth - 1)
		{
			// [1a, 3a, 2b] and [1a, 0b, 2b]
			indexData.push(4 * j + 2, 4 * (j + 1), 4 * (j + 1) + 1);
			indexData.push(4 * j + 2, 4 * j + 3, 4 * (j + 1) + 1);
		}
	}

	// draw front sides of teeth
	// GL_QUADS
	for (i = 0; i < this.teeth; i++, j++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + da));
		geometryData.push(r2 * Math.sin(angle + da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + 2 * da));
		geometryData.push(r2 * Math.sin(angle + 2 * da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle + 3 * da));
		geometryData.push(r1 * Math.sin(angle + 3 * da));
		geometryData.push(this.width * 0.5);

		indexData.push(4 * j, 4 * j + 1, 4 * j + 2);
		indexData.push(4 * j, 4 * j + 2, 4 * j + 3);

		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);
		normalData.push(0.0, 0.0, 1.0);
	}

	// draw back face
	for (i = 0; i <= this.teeth; i+=2, j++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;

		geometryData.push(r1c * Math.cos(angle));
		geometryData.push(r1c * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);

		angle = (i + 1) * 2.0 * Math.PI / this.teeth;

		geometryData.push(r1c * Math.cos(angle));
		geometryData.push(r1c * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);

		indexData.push(4 * j, 4 * j + 2, 4 * j + 3);
		indexData.push(4 * j, 4 * j + 1, 4 * j + 3);

		if (i < this.teeth - 1)
		{
			// [1a, 3a, 2b] and [1a, 0b, 2b]
			indexData.push(4 * j + 2, 4 * (j + 1), 4 * (j + 1) + 1);
			indexData.push(4 * j + 2, 4 * j + 3, 4 * (j + 1) + 1);
		}
	}

	// draw back sides of teeth
	for (i = 0; i < this.teeth; i++, j++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;

		geometryData.push(r1 * Math.cos(angle + 3 * da));
		geometryData.push(r1 * Math.sin(angle + 3 * da));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + 2 * da));
		geometryData.push(r2 * Math.sin(angle + 2 * da));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + da));
		geometryData.push(r2 * Math.sin(angle + da));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		indexData.push(4 * j, 4 * j + 1, 4 * j + 2);
		indexData.push(4 * j, 4 * j + 2, 4 * j + 3);

		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);
		normalData.push(0.0, 0.0, -1.0);
	}

	// draw outward faces of teeth
	var firstId = j;
	for (i = 0; i < this.teeth; i++, j += 2) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;
		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle));
		geometryData.push(r1 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		u = r2 * Math.cos(angle + da) - r1 * Math.cos(angle);
		v = r2 * Math.sin(angle + da) - r1 * Math.sin(angle);
		len = Math.sqrt(u * u + v * v);
		u /= len;
		v /= len;

		normalData.push(v, -u, -0.0);
		normalData.push(v, -u, -0.0);

		geometryData.push(r2 * Math.cos(angle + da));
		geometryData.push(r2 * Math.sin(angle + da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + da));
		geometryData.push(r2 * Math.sin(angle + da));
		geometryData.push(-this.width * 0.5);

		normalData.push(Math.cos(angle), Math.sin(angle), 0.0);
		normalData.push(Math.cos(angle), Math.sin(angle), 0.0);

		geometryData.push(r2 * Math.cos(angle + 2 * da));
		geometryData.push(r2 * Math.sin(angle + 2 * da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r2 * Math.cos(angle + 2 * da));
		geometryData.push(r2 * Math.sin(angle + 2 * da));
		geometryData.push(-this.width * 0.5);

		u = r1 * Math.cos(angle + 3 * da) - r2 * Math.cos(angle + 2 * da);
		v = r1 * Math.sin(angle + 3 * da) - r2 * Math.sin(angle + 2 * da);

		normalData.push(v, -u, 0.0);
		normalData.push(v, -u, 0.0);

		geometryData.push(r1 * Math.cos(angle + 3 * da));
		geometryData.push(r1 * Math.sin(angle + 3 * da));
		geometryData.push(this.width * 0.5);

		geometryData.push(r1 * Math.cos(angle + 3 * da));
		geometryData.push(r1 * Math.sin(angle + 3 * da));
		geometryData.push(-this.width * 0.5);

		normalData.push(Math.cos(angle), Math.sin(angle), 0.0);
		normalData.push(Math.cos(angle), Math.sin(angle), 0.0);

		// first side [0, 2, 3] and [0, 1, 3]
		indexData.push(4 * j, 4 * j + 2, 4 * j + 3);
		indexData.push(4 * j, 4 * j + 1, 4 * j + 3);

		// connect tips [2a, 0b, 1b] and [2a, 1b, 3a]
		indexData.push(4 * j + 2, 4 * (j + 1), 4 * (j + 1) + 1);
		indexData.push(4 * j + 2, 4 * (j + 1) + 1, 4 * j + 3);

		// index [0, 2, 3] and [0, 1, 3]
		indexData.push(4 * (j + 1), 4 * (j + 1) + 2, 4 * (j + 1) + 3);
		indexData.push(4 * (j + 1), 4 * (j + 1) + 1, 4 * (j + 1) + 3);
	
		// second side
		// connect bottom [2b, 0c, 1c] and [2b, 1c, 3b]
		if (i < this.teeth - 1)
		{
			indexData.push(4 * (j + 1) + 2, 4 * (j + 2), 4 * (j + 2) + 1);
			indexData.push(4 * (j + 1) + 2, 4 * (j + 2) + 1, 4 * (j + 1) + 3);
		}
		else
		{
			// close gap between first and last tooth
			indexData.push(4 * (j + 1) + 2, 4 * firstId, 4 * firstId + 1);
			indexData.push(4 * (j + 1) + 2, 4 * firstId + 1, 4 * (j + 1) + 3);
		}
	}

	// draw inside radius cylinder
	for (i = 0; i <= this.teeth; i+=2, j++) 
	{
		angle = i * 2.0 * Math.PI / this.teeth;
		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		normalData.push(-Math.cos(angle), -Math.sin(angle), 0);
		normalData.push(-Math.cos(angle), -Math.sin(angle), 0);

		angle = (i + 1) * 2.0 * Math.PI / this.teeth;

		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(-this.width * 0.5);

		geometryData.push(r0 * Math.cos(angle));
		geometryData.push(r0 * Math.sin(angle));
		geometryData.push(this.width * 0.5);

		// normals
		normalData.push(-Math.cos(angle), -Math.sin(angle), 0);
		normalData.push(-Math.cos(angle), -Math.sin(angle), 0);

		indexData.push(4 * j, 4 * j + 2, 4 * j + 3);
		indexData.push(4 * j, 4 * j + 1, 4 * j + 3);

		if (i < this.teeth - 1)
		{
			indexData.push(4 * j + 2, 4 * (j + 1), 4 * (j + 1) + 1);
			indexData.push(4 * j + 2, 4 * j + 3, 4 * (j + 1) + 1);
		}
	}

	// vertex buffer
	this.vertexObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(geometryData), gl.STATIC_DRAW);
	this.vertexObject.itemSize = 3;
    this.vertexObject.numItems = geometryData.length / this.vertexObject.itemSize;

	// normal buffer
	this.normalObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.normalObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(normalData), gl.STATIC_DRAW);
	this.normalObject.itemSize = 3;
	this.normalObject.numItems = normalData.length / this.normalObject.itemSize;

	// color buffer - same color for all vertices
	var colors = [];
	
	for (var i=0; i<this.vertexObject.numItems; i++)
		colors = colors.concat(this.color);

	this.colorObject = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, this.colorObject);
	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(colors), gl.STATIC_DRAW);
    this.colorObject.itemSize = 4;
    this.colorObject.numItems = this.vertexObject.numItems;

	// index buffer
	this.indexObject = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexObject);
	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(indexData), gl.STATIC_DRAW);
	this.indexObject.itemSize = 1;
	this.indexObject.numItems = indexData.length;
}


/* drawing methods */
/*
Gear.prototype.drawFrontFace = function(gl, shaderProgram) {
	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontFace.vertexObject);
	gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, this.frontFace.vertexObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontFace.colorObject);
	gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, this.frontFace.colorObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontFace.normalObject);
	gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, this.frontFace.normalObject.itemSize, 
							gl.FLOAT, false, 0, 0);

    gl.drawArrays(gl.TRIANGLE_STRIP, 0, this.frontFace.vertexObject.numItems);	
}


Gear.prototype.drawBackFace = function(gl, shaderProgram) {
	gl.bindBuffer(gl.ARRAY_BUFFER, this.backFace.vertexObject);
	gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, this.backFace.vertexObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.backFace.colorObject);
	gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, this.backFace.colorObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.backFace.normalObject);
	gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, this.backFace.normalObject.itemSize, 
							gl.FLOAT, false, 0, 0);

    gl.drawArrays(gl.TRIANGLE_STRIP, 0, this.backFace.vertexObject.numItems);	
}


Gear.prototype.drawFrontTeeth = function(gl, shaderProgram) {
	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontTeeth.vertexObject);
	gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, this.frontTeeth.vertexObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontTeeth.colorObject);
	gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, this.frontTeeth.colorObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.frontTeeth.normalObject);
	gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, this.frontTeeth.normalObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.frontTeeth.indexObject);
	gl.drawElements(gl.TRIANGLES, this.frontTeeth.indexObject.numItems, gl.UNSIGNED_SHORT, 0);
}


Gear.prototype.drawBackTeeth = function(gl, shaderProgram) {
	gl.bindBuffer(gl.ARRAY_BUFFER, this.backTeeth.vertexObject);
	gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, this.backTeeth.vertexObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.backTeeth.colorObject);
	gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, this.backTeeth.colorObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.backTeeth.normalObject);
	gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, this.backTeeth.normalObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.backTeeth.indexObject);
	gl.drawElements(gl.TRIANGLES, this.backTeeth.indexObject.numItems, gl.UNSIGNED_SHORT, 0);
}


Gear.prototype.drawOutwardTeeth = function(gl, shaderProgram) {
	gl.bindBuffer(gl.ARRAY_BUFFER, this.outwardTeeth.vertexObject);
	gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, this.outwardTeeth.vertexObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.outwardTeeth.colorObject);
	gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, this.outwardTeeth.colorObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.outwardTeeth.normalObject);
	gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, this.outwardTeeth.normalObject.itemSize, 
							gl.FLOAT, false, 0, 0);

//	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.outwardTeeth.indexObject);
//	gl.drawElements(gl.TRIANGLES, this.outwardTeeth.indexObject.numItems, gl.UNSIGNED_SHORT, 0);
	gl.drawArrays(gl.TRIANGLE_STRIP, 0, this.outwardTeeth.vertexObject.numItems);	
}


Gear.prototype.drawCylinder = function(gl, shaderProgram) {
	gl.bindBuffer(gl.ARRAY_BUFFER, this.cylinder.vertexObject);
	gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, this.cylinder.vertexObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.cylinder.colorObject);
	gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, this.cylinder.colorObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.cylinder.normalObject);
	gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, this.cylinder.normalObject.itemSize, 
							gl.FLOAT, false, 0, 0);

//	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.cylinder.indexObject);
//	gl.drawElements(gl.TRIANGLES, this.cylinder.indexObject.numItems, gl.UNSIGNED_SHORT, 0);
	gl.drawArrays(gl.TRIANGLE_STRIP, 0, this.cylinder.vertexObject.numItems);	
}
*/


Gear.prototype.draw = function(gl, shaderProgram) {
/*
	this.drawFrontFace(gl, shaderProgram);
	this.drawFrontTeeth(gl, shaderProgram);
	this.drawBackFace(gl, shaderProgram);
	this.drawBackTeeth(gl, shaderProgram);
	this.drawOutwardTeeth(gl, shaderProgram);
	this.drawCylinder(gl, shaderProgram);
*/

	gl.bindBuffer(gl.ARRAY_BUFFER, this.vertexObject);
	gl.vertexAttribPointer(shaderProgram.vertexPositionAttribute, this.vertexObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.colorObject);
	gl.vertexAttribPointer(shaderProgram.vertexColorAttribute, this.colorObject.itemSize, 
							gl.FLOAT, false, 0, 0);

	gl.bindBuffer(gl.ARRAY_BUFFER, this.normalObject);
	gl.vertexAttribPointer(shaderProgram.vertexNormalAttribute, this.normalObject.itemSize, 
							gl.FLOAT, false, 0, 0);

//	gl.drawArrays(gl.LINES, 0, this.vertexObject.numItems);	

	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indexObject);
	gl.drawElements(gl.TRIANGLES, this.indexObject.numItems, gl.UNSIGNED_SHORT, 0);
}

