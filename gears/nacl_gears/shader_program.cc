// Copyright 2011 Dennis Jarosch.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "shader_program.h"

#include <assert.h>
//_______________________________________________________________________

ShaderProgram::ShaderProgram()
	:	_shaderProgram(0),
		vertexPositionAttribute(0),
		vertexNormalAttribute(0),
		vertexColorAttribute(0),
		lightingDirectionUniform(0),
		directionalColorUniform(0),
		pMatrixUniform(0),
		mvMatrixUniform(0),
		nMatrixUniform(0)
{
}
//_______________________________________________________________________

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_shaderProgram);
}
//_______________________________________________________________________

GLuint ShaderProgram::CreateShader(GLenum type, const char *shaderSrc)
{
	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == 0) 
	{

		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) 
		{
			char* infoLog = new char[infoLen];

			//reinterpret_cast<char*>(malloc(sizeof(char) * infoLen));
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);

			printf("Error compiling shader:\n%s\n", infoLog);
			delete[] infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;	
}
//_______________________________________________________________________

void ShaderProgram::InitShaders(void)
{

	const char vertexShaderSrc[] =  
		"attribute vec3 aVertexPosition;																\n"
		"attribute vec4 aVertexColor;																	\n"
		"attribute vec3 aVertexNormal;																	\n"
		"uniform mat4 uMVMatrix;																		\n"
		"uniform mat4 uPMatrix;																			\n"
		"uniform mat4 uNMatrix;																			\n"
		"uniform vec3 uLightingDirection;																\n"
	  	"uniform vec3 uDirectionalColor;																\n"
		"varying vec4 vColor;																			\n"
		"varying vec3 vLightWeighting;																	\n"
		"void main(void) 																				\n"
		"{																								\n"
		"	gl_Position = uPMatrix * uMVMatrix * vec4(aVertexPosition, 1.0);							\n"
		"	vColor = aVertexColor;																		\n"
		"	vec4 transformedNormal = uNMatrix * vec4(aVertexNormal, 1.0);								\n"
		"	float directionalLightWeighting = max(dot(transformedNormal.xyz, uLightingDirection), 0.0); \n"
		"	vLightWeighting = uDirectionalColor * directionalLightWeighting;							\n"
		"}																								\n";

	const char fragmentShaderSrc[] =
		"#ifdef GL_ES														\n"
		"	precision highp float;											\n"
		"#endif																\n"
		"varying vec4 vColor;												\n"
		"varying vec3 vLightWeighting;										\n"
		"void main(void) 													\n"
		"{																	\n"
		"	gl_FragColor = vec4(vColor.rgb * vLightWeighting, vColor.a);	\n"
		"}																	\n";


	// create shaders
	GLuint vertexShader 	= CreateShader(GL_VERTEX_SHADER, vertexShaderSrc);
	GLuint fragmentShader 	= CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

	assert(vertexShader != 0);
	assert(fragmentShader != 0);

	// create shader program, attach shaders, and link
	_shaderProgram = glCreateProgram();

	assert(_shaderProgram != 0);

	glAttachShader(_shaderProgram, vertexShader);
	glAttachShader(_shaderProgram, fragmentShader);

	glLinkProgram(_shaderProgram);
	
	// delete shaders, as they are attached to the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// check link status
	GLint status;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &status);
	
	if (status == 0) 
	{
		GLint infoLen = 0;
		glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) 
		{
			char* infoLog = new char[infoLen];
			//reinterpret_cast<char*>(malloc(sizeof(char) * info_len));
			glGetProgramInfoLog(_shaderProgram, infoLen, NULL, infoLog);
			printf("Error linking shader program:\n%s\n", infoLog);

			delete[] infoLog;
		}
		glDeleteProgram(_shaderProgram);
		return;
	}

	glUseProgram(_shaderProgram);

	vertexPositionAttribute = glGetAttribLocation(_shaderProgram, "aVertexPosition");
	glEnableVertexAttribArray(vertexPositionAttribute);

	vertexNormalAttribute = glGetAttribLocation(_shaderProgram, "aVertexNormal");
	glEnableVertexAttribArray(vertexNormalAttribute);

	vertexColorAttribute = glGetAttribLocation(_shaderProgram, "aVertexColor");
    glEnableVertexAttribArray(vertexColorAttribute);

	pMatrixUniform  = glGetUniformLocation(_shaderProgram, "uPMatrix");
	mvMatrixUniform = glGetUniformLocation(_shaderProgram, "uMVMatrix");
	nMatrixUniform  = glGetUniformLocation(_shaderProgram, "uNMatrix");

	// lighting
	lightingDirectionUniform = glGetUniformLocation(_shaderProgram, "uLightingDirection");
    directionalColorUniform  = glGetUniformLocation(_shaderProgram, "uDirectionalColor");
}
//_______________________________________________________________________

GLuint ShaderProgram::GetShaderProgram(void)
{
	return _shaderProgram;
}
//_______________________________________________________________________
