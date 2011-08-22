// Copyright 2011 Dennis Jarosch.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "gear.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;
//_______________________________________________________________________

Buffer::Buffer()
	:	itemSize(0),
		numItems(0)
{
	glGenBuffers(1, &buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &buffer);
}

//_______________________________________________________________________
//_______________________________________________________________________

Gear::Gear(float innerRadius, 
		  float outerRadius, 
		  float width, 
		  int teeth, 
		  float toothDepth, 
		  GLfloat color[4])
	:	_innerRadius(innerRadius),
		_outerRadius(outerRadius),
		_width(width),
		_teeth(teeth),
		_toothDepth(toothDepth)
{
	memcpy(_color, color, 4 * sizeof(GLfloat));

	CreateAll();
}
//_______________________________________________________________________

Gear::~Gear()
{
}
//_______________________________________________________________________

void Gear::CreateAll(void)
{
	vector<GLfloat>  geometryData;
	vector<GLfloat>  normalData;
	vector<GLushort> indexData;

	int i, j = 0;
	float angle;
	float u, v, len;

	float r0 = _innerRadius;
	float r1 = _outerRadius - _toothDepth / 2.0;
	float r2 = _outerRadius + _toothDepth / 2.0;
	// increase outer radius to resolve drawing errors for front and back faces
	float r1c = r1 + 0.05;

	float da = 2.0 * M_PI / _teeth / 4.0;	

	// draw front face
	for (i = 0; i <= _teeth; i+=2, j++) 
	{
		angle = i * 2.0 * M_PI / _teeth;

		geometryData.push_back(r0 * cos(angle));
		geometryData.push_back(r0 * sin(angle)); 
		geometryData.push_back(_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(1.0);

		geometryData.push_back(r1c * cos(angle));
		geometryData.push_back(r1c * sin(angle));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(1.0);

		angle = (i + 1) * 2.0 * M_PI / _teeth;

		geometryData.push_back(r0 * cos(angle));
		geometryData.push_back(r0 * sin(angle)); 
		geometryData.push_back(_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(1.0);

		geometryData.push_back(r1c * cos(angle));
		geometryData.push_back(r1c * sin(angle));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(1.0);

		// [0, 2, 3] and [0, 1, 3]
		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 2);
		indexData.push_back(4 * j + 3);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 1);
		indexData.push_back(4 * j + 3);
		
		if (i < _teeth - 1)
		{
			// [1a, 3a, 2b] and [1a, 0b, 2b]
			indexData.push_back(4 * j + 2);
			indexData.push_back(4 * (j + 1));
			indexData.push_back(4 * (j + 1) + 1);

			indexData.push_back(4 * j + 2);
			indexData.push_back(4 * j + 3);
			indexData.push_back(4 * (j + 1) + 1);
		}
	}

	// draw front sides of teeth
	for (int i = 0; i < _teeth; i++, j++) 
	{
		angle = i * 2.0 * M_PI / _teeth;

		geometryData.push_back(r1 * cos(angle));
		geometryData.push_back(r1 * sin(angle));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(1.0);

		geometryData.push_back(r2 * cos(angle + da));
		geometryData.push_back(r2 * sin(angle + da));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(1.0);

		geometryData.push_back(r2 * cos(angle + 2 * da));
		geometryData.push_back(r2 * sin(angle + 2 * da));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(1.0);

		geometryData.push_back(r1 * cos(angle + 3 * da));
		geometryData.push_back(r1 * sin(angle + 3 * da));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(1.0);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 1);
		indexData.push_back(4 * j + 2);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 2);
		indexData.push_back(4 * j + 3);
	}

	// draw back face
	for (i = 0; i <= _teeth; i+=2, j++) 
	{
		angle = i * 2.0 * M_PI / _teeth;

		geometryData.push_back(r1c * cos(angle));
		geometryData.push_back(r1c * sin(angle));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(-1.0);

		geometryData.push_back(r0 * cos(angle));
		geometryData.push_back(r0 * sin(angle));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(-1.0);

		angle = (i + 1) * 2.0 * M_PI / _teeth;

		geometryData.push_back(r1c * cos(angle));
		geometryData.push_back(r1c * sin(angle));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(-1.0);

		geometryData.push_back(r0 * cos(angle));
		geometryData.push_back(r0 * sin(angle));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(-1.0);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 2);
		indexData.push_back(4 * j + 3);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 1);
		indexData.push_back(4 * j + 3);

		if (i < _teeth - 1)
		{
			// [1a, 3a, 2b] and [1a, 0b, 2b]
			indexData.push_back(4 * j + 2);
			indexData.push_back(4 * (j + 1));
			indexData.push_back(4 * (j + 1) + 1);

			indexData.push_back(4 * j + 2);
			indexData.push_back(4 * j + 3);
			indexData.push_back(4 * (j + 1) + 1);
		}
	}

	// draw back sides of teeth
	for (i = 0; i < _teeth; i++, j++) 
	{
		angle = i * 2.0 * M_PI / _teeth;

		geometryData.push_back(r1 * cos(angle + 3 * da));
		geometryData.push_back(r1 * sin(angle + 3 * da));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(-1.0);

		geometryData.push_back(r2 * cos(angle + 2 * da));
		geometryData.push_back(r2 * sin(angle + 2 * da));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(-1.0);

		geometryData.push_back(r2 * cos(angle + da));
		geometryData.push_back(r2 * sin(angle + da));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(-1.0);

		geometryData.push_back(r1 * cos(angle));
		geometryData.push_back(r1 * sin(angle));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(0.0);
		normalData.push_back(0.0);
		normalData.push_back(-1.0);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 1);
		indexData.push_back(4 * j + 2);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 2);
		indexData.push_back(4 * j + 3);
	}

	// draw outward faces of teeth
	int firstId = j;
	
	for (i = 0; i < _teeth; i++, j += 2) 
	{
		angle = i * 2.0 * M_PI / _teeth;
		geometryData.push_back(r1 * cos(angle));
		geometryData.push_back(r1 * sin(angle));
		geometryData.push_back(_width * 0.5);

		u = r2 * cos(angle + da) - r1 * cos(angle);
		v = r2 * sin(angle + da) - r1 * sin(angle);
		len = sqrt(u * u + v * v);
		u /= len;
		v /= len;

		normalData.push_back(v);
		normalData.push_back(-u);
		normalData.push_back(-0.0);

		geometryData.push_back(r1 * cos(angle));
		geometryData.push_back(r1 * sin(angle));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(v);
		normalData.push_back(-u);
		normalData.push_back(-0.0);

		geometryData.push_back(r2 * cos(angle + da));
		geometryData.push_back(r2 * sin(angle + da));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(cos(angle));
		normalData.push_back(sin(angle));
		normalData.push_back(0.0);

		geometryData.push_back(r2 * cos(angle + da));
		geometryData.push_back(r2 * sin(angle + da));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(cos(angle));
		normalData.push_back(sin(angle));
		normalData.push_back(0.0);

		geometryData.push_back(r2 * cos(angle + 2 * da));
		geometryData.push_back(r2 * sin(angle + 2 * da));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(v);
		normalData.push_back(-u);
		normalData.push_back(0.0);

		geometryData.push_back(r2 * cos(angle + 2 * da));
		geometryData.push_back(r2 * sin(angle + 2 * da));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(v);
		normalData.push_back(-u);
		normalData.push_back(0.0);

		u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
		v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);

		geometryData.push_back(r1 * cos(angle + 3 * da));
		geometryData.push_back(r1 * sin(angle + 3 * da));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(cos(angle));
		normalData.push_back(sin(angle));
		normalData.push_back(0.0);

		geometryData.push_back(r1 * cos(angle + 3 * da));
		geometryData.push_back(r1 * sin(angle + 3 * da));
		geometryData.push_back(-_width * 0.5);

		normalData.push_back(cos(angle));
		normalData.push_back(sin(angle));
		normalData.push_back(0.0);

		// first side [0, 2, 3] and [0, 1, 3]
		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 2);
		indexData.push_back(4 * j + 3);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 1);
		indexData.push_back(4 * j + 3);

		// connect tips [2a, 0b, 1b] and [2a, 1b, 3a]
		indexData.push_back(4 * j + 2);
		indexData.push_back(4 * (j + 1));
		indexData.push_back(4 * (j + 1) + 1);

		indexData.push_back(4 * j + 2);
		indexData.push_back(4 * (j + 1) + 1);
		indexData.push_back(4 * j + 3);

		// index [0, 2, 3] and [0, 1, 3]
		indexData.push_back(4 * (j + 1));
		indexData.push_back(4 * (j + 1) + 2);
		indexData.push_back(4 * (j + 1) + 3);

		indexData.push_back(4 * (j + 1));
		indexData.push_back(4 * (j + 1) + 1);
		indexData.push_back(4 * (j + 1) + 3);
	
		// second side
		// connect bottom [2b, 0c, 1c] and [2b, 1c, 3b]
		if (i < _teeth - 1)
		{
			indexData.push_back(4 * (j + 1) + 2);
			indexData.push_back(4 * (j + 2));
			indexData.push_back(4 * (j + 2) + 1);

			indexData.push_back(4 * (j + 1) + 2);
			indexData.push_back(4 * (j + 2) + 1);
			indexData.push_back(4 * (j + 1) + 3);
		}
		else
		{
			// close gap between first and last tooth
			indexData.push_back(4 * (j + 1) + 2);
			indexData.push_back(4 * firstId);
			indexData.push_back(4 * firstId + 1);

			indexData.push_back(4 * (j + 1) + 2);
			indexData.push_back(4 * firstId + 1);
			indexData.push_back(4 * (j + 1) + 3);
		}
	}

	// draw inside radius cylinder
	for (i = 0; i <= _teeth; i+=2, j++) 
	{
		angle = i * 2.0 * M_PI / _teeth;
		geometryData.push_back(r0 * cos(angle));
		geometryData.push_back(r0 * sin(angle));
		geometryData.push_back(-_width * 0.5);

		geometryData.push_back(r0 * cos(angle));
		geometryData.push_back(r0 * sin(angle));
		geometryData.push_back(_width * 0.5);

		normalData.push_back(-cos(angle));
		normalData.push_back(-sin(angle));
		normalData.push_back(0);

		normalData.push_back(-cos(angle));
		normalData.push_back(-sin(angle));
		normalData.push_back(0);

		angle = (i + 1) * 2.0 * M_PI / _teeth;

		geometryData.push_back(r0 * cos(angle));
		geometryData.push_back(r0 * sin(angle));
		geometryData.push_back(-_width * 0.5);

		geometryData.push_back(r0 * cos(angle));
		geometryData.push_back(r0 * sin(angle));
		geometryData.push_back(_width * 0.5);

		// normals
		normalData.push_back(-cos(angle));
		normalData.push_back(-sin(angle));
		normalData.push_back(0);

		normalData.push_back(-cos(angle));
		normalData.push_back(-sin(angle));
		normalData.push_back(0);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 2);
		indexData.push_back(4 * j + 3);

		indexData.push_back(4 * j);
		indexData.push_back(4 * j + 1);
		indexData.push_back(4 * j + 3);

		if (i < _teeth - 1)
		{
			indexData.push_back(4 * j + 2);
			indexData.push_back(4 * (j + 1));
			indexData.push_back(4 * (j + 1) + 1);

			indexData.push_back(4 * j + 2);
			indexData.push_back(4 * j + 3);
			indexData.push_back(4 * (j + 1) + 1);
		}
	}

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuf.buffer);
	glBufferData(GL_ARRAY_BUFFER,
               geometryData.size() * sizeof(GLfloat),
               &geometryData[0],
               GL_STATIC_DRAW);
	_vertexBuf.itemSize = 3;
    _vertexBuf.numItems = geometryData.size() / _vertexBuf.itemSize;

	// normal buffer
	glBindBuffer(GL_ARRAY_BUFFER, _normalBuf.buffer);
	glBufferData(GL_ARRAY_BUFFER,
		       normalData.size() * sizeof(GLfloat),
		       &normalData[0],
		       GL_STATIC_DRAW);
	_normalBuf.itemSize = 3;
    _normalBuf.numItems = normalData.size() / _normalBuf.itemSize;

	// color buffer
	int colCount = _vertexBuf.numItems * 4;
	GLfloat colorData[colCount];

	for (int i = 0; i < colCount; i += 4)
		memcpy(&colorData[i], _color, 4 * sizeof(GLfloat));

	glBindBuffer(GL_ARRAY_BUFFER, _colorBuf.buffer);
	glBufferData(GL_ARRAY_BUFFER,
		       _vertexBuf.numItems * 4 * sizeof(GLfloat),
		       colorData,
		       GL_STATIC_DRAW);
	_colorBuf.itemSize = 4;
    _colorBuf.numItems = _vertexBuf.numItems;

	// index buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuf.buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		       indexData.size() * sizeof(GL_UNSIGNED_SHORT),
		       &indexData[0],
		       GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	_indexBuf.itemSize = 1;
    _indexBuf.numItems = indexData.size();
}
//_______________________________________________________________________

void Gear::Draw(ShaderProgram &shaderProgram)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuf.buffer);
	glVertexAttribPointer(shaderProgram.vertexPositionAttribute, _vertexBuf.itemSize, 
							GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, _colorBuf.buffer);
	glVertexAttribPointer(shaderProgram.vertexColorAttribute, _colorBuf.itemSize, 
							GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, _normalBuf.buffer);
	glVertexAttribPointer(shaderProgram.vertexNormalAttribute, _normalBuf.itemSize, 
							GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuf.buffer);
	glDrawElements(GL_TRIANGLES, _indexBuf.numItems, GL_UNSIGNED_SHORT, 0);
}
//_______________________________________________________________________

