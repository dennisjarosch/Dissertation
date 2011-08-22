// Copyright 2011 Dennis Jarosch.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <pgl/pgl.h>
#include <GLES2/gl2.h>

//_______________________________________________________________________

class ShaderProgram
{
private:
	GLuint 				_shaderProgram;

	GLuint		CreateShader(GLenum type, const char *shaderSrc);

public:
	GLint				vertexPositionAttribute;
	GLint				vertexNormalAttribute;
	GLint				vertexColorAttribute;
	GLint				lightingDirectionUniform;
	GLint				directionalColorUniform;
	GLint				pMatrixUniform;
	GLint				mvMatrixUniform;
	GLint				nMatrixUniform;

	ShaderProgram();
	~ShaderProgram();

	void		InitShaders(void);

	GLuint		GetShaderProgram(void);
};

//_______________________________________________________________________

#endif
