/*
  Copyright 2010 The Native Client SDK Authors. All rights reserved.

  Copyright 2010, Google Inc.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

	* Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following disclaimer
      in the documentation and/or other materials provided with the
      distribution.
	* Neither the name of Google Inc. nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef EXAMPLES_TUMBLER_TRANSFORMS_H_
#define EXAMPLES_TUMBLER_TRANSFORMS_H_

#include <GLES2/gl2.h>

// A very simple set of 4x4 matrix routines.  In all these routines, the input
// matrix is assumed to be a 4x4 of GLfloats.

namespace transform_4x4 {

// Pre-multply |m| with a projection transformation 4x4 matrix from a
// truncated pyramid viewing frustum.
void Frustum(GLfloat* m,
             GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat near_z,
             GLfloat far_z);

// Replace |m| with the 4x4 identity matrix.
void LoadIdentity(GLfloat* m);

// |m| <- |a| . |b|.  |m| can point at the same memory as either |a| or |b|.
void Multiply(GLfloat *m, GLfloat *a, GLfloat* b);

// Pre-multiply |m| with a single-point perspective matrix based on the viewing
// frustum whose view angle is |fovy|.
void Perspective(GLfloat* m, 
                 GLfloat fovy,
                 GLfloat aspect,
                 GLfloat near_z,
                 GLfloat far_z);

// Pre-multiply |m| with a matrix that represents a translation by |tx|, |ty|,
// |tz|.
void Translate(GLfloat* m, GLfloat tx, GLfloat ty, GLfloat tz);
void Rotate(GLfloat m[16], GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

void Transpose(GLfloat mOut[16], GLfloat mIn[16]);
void Inverse(GLfloat mOut[16], GLfloat mIn[16]);

}  // transform_4x4

#endif  // EXAMPLES_TUMBLERTRANSFORMS_H_
