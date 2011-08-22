// Copyright 2011 Dennis Jarosch.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#ifndef GEARS_H
#define GEARS_H

#include <nacl/nacl_npapi.h>
#include <pgl/pgl.h>
#include <GLES2/gl2.h>
#include <stack>

#include "shader_program.h"
#include "gear.h"

//_______________________________________________________________________

class NaclGears
{
private:
	NPP					_npp;
	NPObject* 			_npObj;
	NPDevice*			_device3D;
	NPDeviceContext3D	_context3D;
	PGLContext 			_contextPGL;
	int					_width;
	int					_height;
	char				_keysPressed;
	int					_t0;
	int					_frames;
	ShaderProgram		_shaderProgram;
	GLfloat				_pMatrix[16];
	GLfloat				_mvMatrix[16];
	GLfloat 			_viewRotx;
	GLfloat				_viewRoty;
	GLfloat				_viewRotz;
	GLfloat				_angle;
	Gear*				_gear1, *_gear2, *_gear3;

	void		CreateContext(void);
	void		SetMatrixUniforms(GLfloat *mvMatrix);
	int			GetCurrentTime(void);
	void		HandleKeyPress(void);
	bool 		DrawScene(void);
	
public:
	explicit NaclGears(NPP npp);
	~NaclGears();

	NPObject* 	GetScriptableObject(void);
	NPError 	SetWindow(const NPWindow& window);

	void		InitGL(void);
	void		PostRedrawNotification(void);
	bool		KeyDown(int keyCode);
	bool		KeyUp(int keyCode);

	void		EventLoop(void);
};

//_______________________________________________________________________

#endif
