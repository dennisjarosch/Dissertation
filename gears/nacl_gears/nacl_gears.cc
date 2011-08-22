// Copyright 2011 Dennis Jarosch.
// Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "nacl_gears.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <nacl/npapi_extensions.h>
#include <nacl/npupp.h>
#include <nacl/npruntime.h>
#include <sys/time.h>

#include "scripting_bridge.h"
#include "transforms.h"

const int32_t 	cCommandBufferSize = 1024 * 1024;
const GLfloat   cLightPos[] = {0.5, 0.5, 1.0}; 
const char		cKeyLeft  = 0x00000001;
const char		cKeyRight = 0x00000002;
const char		cKeyUp    = 0x00000004;
const char		cKeyDown  = 0x00000008;

// declared in npn_bridge.c, used to get the 3D device from the pepper interface
extern NPDevice* NPN_AcquireDevice(NPP instance, NPDeviceID device);

void EventLoopCallback(void* data) 
{
	static_cast<NaclGears*>(data)->EventLoop();
}

//_______________________________________________________________________

NaclGears::NaclGears(NPP npp)
	:	_npp(npp),
		_npObj(NULL),
		_device3D(NULL),
		_contextPGL(NULL),
		_width(400),
		_height(400),
		_keysPressed(0),
		_t0(-1),
		_frames(0),
		_viewRotx(20.0),
		_viewRoty(30.0),
		_viewRotz(0.0),
		_angle(0.0),
		_gear1(NULL)
{
	ScriptingBridge::Init();

	memset(&_context3D, 0, sizeof(_context3D));

	CreateContext();
}

//_______________________________________________________________________

NaclGears::~NaclGears()
{  
	delete _gear1;
	delete _gear2;
	delete _gear3;
	
	if (_contextPGL)
	{
		pglDestroyContext(_contextPGL);
		_contextPGL = NULL;
	}

	_device3D->destroyContext(_npp, &_context3D);

	if (_npObj)
		NPN_ReleaseObject(_npObj);
}
//_______________________________________________________________________

void NaclGears::CreateContext(void)
{
	if (!_contextPGL)
	{
		// Create and initialize 3D and PGL contexts.
		_device3D = NPN_AcquireDevice(_npp, NPPepper3DDevice);
		assert(NULL != _device3D);

		NPDeviceContext3DConfig config;
		config.commandBufferSize = cCommandBufferSize;

		_device3D->initializeContext(_npp, &config, &_context3D);
		_contextPGL = pglCreateContext(_npp, _device3D, &_context3D);
		assert(NULL != _contextPGL);
	}
}
//_______________________________________________________________________

void NaclGears::SetMatrixUniforms(GLfloat *mvMatrix)
{
	glUniformMatrix4fv(_shaderProgram.pMatrixUniform, 1, GL_FALSE, _pMatrix);
	glUniformMatrix4fv(_shaderProgram.mvMatrixUniform, 1, GL_FALSE, mvMatrix);

	GLfloat normalMatrix[16];
	GLfloat transposedMatrix[16];

	transform_4x4::Inverse(normalMatrix, mvMatrix);
	transform_4x4::Transpose(transposedMatrix, normalMatrix);
	 
	glUniformMatrix4fv(_shaderProgram.nMatrixUniform, 1, 0, transposedMatrix);	
}
//_______________________________________________________________________

int NaclGears::GetCurrentTime(void)
{
	struct timeval tv;
	struct timezone tz;

	gettimeofday(&tv, &tz);
	return (int) tv.tv_sec;
}
//_______________________________________________________________________

void NaclGears::HandleKeyPress(void)
{
	if (_keysPressed & cKeyLeft)
		_viewRoty += 0.1;
	if (_keysPressed & cKeyRight)
		_viewRoty -= 0.1;
	if (_keysPressed & cKeyUp)
		_viewRotx += 0.1;
	if (_keysPressed & cKeyDown)
		_viewRotx -= 0.1;
}
//_______________________________________________________________________

bool NaclGears::DrawScene(void)
{
	static int skips = 0;

	if (++skips < 100)
		return true;

	skips = 0;

	if (!pglMakeCurrent(_contextPGL))
	{
		return false;
		printf("DrawScene: pglMakeCurrent() failed\n");
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	transform_4x4::LoadIdentity(_pMatrix);
	transform_4x4::Perspective(_pMatrix, 25.0f, _width / _height, 5.0f, 60.0f);

	// lighting
	glUniform3fv(_shaderProgram.lightingDirectionUniform, 1, cLightPos);
	glUniform3f(_shaderProgram.directionalColorUniform, 1.0, 1.0, 1.0);

	transform_4x4::LoadIdentity(_mvMatrix);
	transform_4x4::Translate(_mvMatrix, 0.0, 0.0, -40.0);
	transform_4x4::Rotate(_mvMatrix, _viewRotx, 1.0, 0.0, 0.0);
	transform_4x4::Rotate(_mvMatrix, _viewRoty, 0.0, 1.0, 0.0);
 
	// gear 1
	GLfloat gearMatrix[16];
	memcpy(gearMatrix, _mvMatrix, 16 * sizeof(GLfloat));
	transform_4x4::Translate(gearMatrix, -3.0, -2.0, 0.0);
	transform_4x4::Rotate(gearMatrix, _angle, 0.0, 0.0, 1.0);
	SetMatrixUniforms(gearMatrix);

	_gear1->Draw(_shaderProgram);
	memset(gearMatrix, 0, 16 * sizeof(GLfloat));

	// gear 2
	memcpy(gearMatrix, _mvMatrix, 16 * sizeof(GLfloat));
	transform_4x4::Translate(gearMatrix, 3.1, -2.0, 0.0);
	transform_4x4::Rotate(gearMatrix, -2.0 * _angle - 9.0, 0.0, 0.0, 1.0);
	SetMatrixUniforms(gearMatrix);

	_gear2->Draw(_shaderProgram);
	memset(gearMatrix, 0, 16 * sizeof(GLfloat));

	// gear 3
	memcpy(gearMatrix, _mvMatrix, 16 * sizeof(GLfloat));
	transform_4x4::Translate(gearMatrix, -3.1, 4.2, 0.0);
	transform_4x4::Rotate(gearMatrix, -2.0 * _angle - 25.0, 0.0, 0.0, 1.0);
	SetMatrixUniforms(gearMatrix);

	_gear3->Draw(_shaderProgram);

	pglSwapBuffers();
	pglMakeCurrent(PGL_NO_CONTEXT);

	return true;
}
//_______________________________________________________________________

NPObject* NaclGears::GetScriptableObject(void)
{
	if (!_npObj)
		_npObj = NPN_CreateObject(_npp, &ScriptingBridge::np_class);
	else
		NPN_RetainObject(_npObj);

	return _npObj;
}
//_______________________________________________________________________

NPError NaclGears::SetWindow(const NPWindow& window)
{
	_width  = window.width;
	_height = window.height;
/*
	if (!_contextPGL)
		CreateContext();
*/
	if (!pglMakeCurrent(_contextPGL))
	    return NPERR_INVALID_INSTANCE_ERROR;

	InitGL();

	PostRedrawNotification();

	return NPERR_NO_ERROR;
}
//_______________________________________________________________________

void NaclGears::InitGL(void)
{
	_shaderProgram.InitShaders();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	
	glViewport(0, 0, _width, _height);

	GLfloat col1[4] = {0.8, 0.1, 0.0, 1.0};
	_gear1 = new Gear(1.0, 4.0, 1.0, 20, 0.7, col1);

	GLfloat col2[4] = {0.0, 0.8, 0.2, 1.0};
	_gear2 = new Gear(0.5, 2.0, 2.0, 10, 0.7, col2);

	GLfloat col3[4] = {0.2, 0.2, 1.0, 1.0};
	_gear3 = new Gear(1.3, 2.0, 0.5, 10, 0.7, col3);
}
//_______________________________________________________________________

void NaclGears::PostRedrawNotification() 
{
	NPN_PluginThreadAsyncCall(_npp, EventLoopCallback, this);
}
//_______________________________________________________________________

bool NaclGears::KeyDown(int keyCode)
{
	switch(keyCode)
	{
		case 37:
			_keysPressed |= cKeyLeft;	
		break;

		case 39:
			_keysPressed |= cKeyRight;	
		break;

		case 38:
			_keysPressed |= cKeyUp;	
		break;

		case 40:
			_keysPressed |= cKeyDown;	
		break;
	}

	return true;
}
//_______________________________________________________________________

bool NaclGears::KeyUp(int keyCode)
{
	switch(keyCode)
	{
		case 37:
			_keysPressed ^= cKeyLeft;	
		break;

		case 39:
			_keysPressed ^= cKeyRight;	
		break;

		case 38:
			_keysPressed ^= cKeyUp;	
		break;

		case 40:
			_keysPressed ^= cKeyDown;	
		break;
	}

	return true;
}
//_______________________________________________________________________

void NaclGears::EventLoop(void)
{
	_angle = _angle + 2.0;

	HandleKeyPress();

	DrawScene();
	PostRedrawNotification();

	// calc framerate
	int t = GetCurrentTime();

	if (_t0 < 0)
		_t0 = t;

	_frames++;

	if (t - _t0 >= 5.0)
	{
		int secs = t - _t0;
		int fps  = _frames / secs;
	
		printf("%d frames in %d seconds = %d fps\n", _frames, secs, fps);

		_t0		= t;
		_frames = 0;
	}
}
//_______________________________________________________________________
