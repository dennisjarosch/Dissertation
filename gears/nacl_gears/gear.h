// Copyright 2011 Dennis Jarosch.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.


#ifndef GEAR_H
#define GEAR_H

#include <pgl/pgl.h>
#include <GLES2/gl2.h>

#include "shader_program.h"
//_______________________________________________________________________

class Buffer
{
public:
	Buffer();
	~Buffer();
	
	GLuint 	buffer;
	int		itemSize;
	int		numItems;
};

//_______________________________________________________________________

//_______________________________________________________________________

class Gear
{
private:
	float	_innerRadius;
	float	_outerRadius;
	float	_width;
	int		_teeth;
	float	_toothDepth;
	GLfloat	_color[4];

	Buffer	_vertexBuf;
	Buffer	_normalBuf;
	Buffer	_colorBuf;
	Buffer	_indexBuf;

public:
	Gear(float innerRadius, 
		 float outerRadius, 
		 float width, 
		 int teeth, 
		 float toothDepth, 
		 GLfloat color[4]);
	~Gear();

	void CreateAll(void);
	void Draw(ShaderProgram &shaderProgram);
};

//_______________________________________________________________________

#endif
