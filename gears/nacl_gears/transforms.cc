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

#include "examples/tumbler/transforms.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <GLES2/gl2.h>

namespace transform_4x4 {

static const GLfloat kPI = 3.1415926535897932384626433832795f;

/*
static void PrintMatrix(GLfloat *m, int nRows, int nCols) {
  for (int i = 0; i < nRows; i++) {
    for (int j = 0; j < nCols; j++) {
      printf("%.2f ", m[i + nRows * j]);
    }
    printf("\n");
  }
}
*/

void Translate(GLfloat* m, GLfloat tx, GLfloat ty, GLfloat tz) {
  m[12] += (m[0] * tx + m[4] * ty + m[8] * tz);
  m[13] += (m[1] * tx + m[5] * ty + m[9] * tz);
  m[14] += (m[2] * tx + m[6] * ty + m[10] * tz);
  m[15] += (m[3] * tx + m[7] * ty + m[11] * tz);
}

void Rotate(GLfloat m[16], GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
  GLfloat angleRad = angle * M_PI / 180.0;
  GLfloat rotMat[16];
  GLfloat s = sin(angleRad);
  GLfloat c = cos(angleRad);
  GLfloat t = 1 - c;

  rotMat[0] = t * x * x + c;
  rotMat[1] = t * x * y + s * z;
  rotMat[2] = t * x * z - s * y;
  rotMat[3] = 0;

  rotMat[4] = t * x * y - s * z;
  rotMat[5] = t * y * y + c;
  rotMat[6] = t * y * z + s * x;
  rotMat[7] = 0;

  rotMat[8] = t * x * z + s * y;
  rotMat[9] = t * y * z - s * x;
  rotMat[10] = t * z * z + c;
  rotMat[11] = 0;

  rotMat[12] = 0;
  rotMat[13] = 0;
  rotMat[14] = 0;
  rotMat[15] = 1;

  Multiply(m, rotMat, m);  
}

void Frustum(GLfloat* m,
             GLfloat left,
             GLfloat right,
             GLfloat bottom,
             GLfloat top,
             GLfloat near_z,
             GLfloat far_z) {
  GLfloat delta_x = right - left;
  GLfloat delta_y = top - bottom;
  GLfloat delta_z = far_z - near_z;
  GLfloat frustum[16];

  if ((near_z <= 0.0f) || (far_z <= 0.0f) ||
     (delta_x <= 0.0f) || (delta_y <= 0.0f) || (delta_z <= 0.0f))
     return;

  frustum[0] = 2.0f * near_z / delta_x;
  frustum[1] = frustum[2] = frustum[3] = 0.0f;

  frustum[5] = 2.0f * near_z / delta_y;
  frustum[4] = frustum[6] = frustum[7] = 0.0f;

  frustum[8] = (right + left) / delta_x;
  frustum[9] = (top + bottom) / delta_y;
  frustum[10] = -(near_z + far_z) / delta_z;
  frustum[11] = -1.0f;

  frustum[14] = -2.0f * near_z * far_z / delta_z;
  frustum[12] = frustum[13] = frustum[15] = 0.0f;

  transform_4x4::Multiply(m, frustum, m);
}


void Perspective(GLfloat* m, 
                 GLfloat fovy,
                 GLfloat aspect,
                 GLfloat near_z,
                 GLfloat far_z) {
  GLfloat frustum_w, frustum_h;

//  frustum_h = tanf((fovy * 0.5f) / 180.0f * kPI) * near_z;
  frustum_h = near_z * tan(fovy * M_PI / 360.0f);
  frustum_w = frustum_h * aspect;
  transform_4x4::Frustum(m, -frustum_w, frustum_w, -frustum_h, frustum_h,
                         near_z, far_z);
}

void Multiply(GLfloat *m, GLfloat *a, GLfloat* b) {
  GLfloat tmp[16];
  // tmp = a . b
  GLfloat a0, a1, a2, a3;
  a0 = a[0]; a1 = a[1]; a2 = a[2]; a3 = a[3];
  tmp[0] = a0 * b[0] + a1 * b[4] + a2 * b[8] + a3 * b[12];
  tmp[1] = a0 * b[1] + a1 * b[5] + a2 * b[9] + a3 * b[13];
  tmp[2] = a0 * b[2] + a1 * b[6] + a2 * b[10] + a3 * b[14];
  tmp[3] = a0 * b[3] + a1 * b[7] + a2 * b[11] + a3 * b[15];

  a0 = a[4]; a1 = a[5]; a2 = a[6]; a3 = a[7];
  tmp[4] = a0 * b[0] + a1 * b[4] + a2 * b[8] + a3 * b[12];
  tmp[5] = a0 * b[1] + a1 * b[5] + a2 * b[9] + a3 * b[13];
  tmp[6] = a0 * b[2] + a1 * b[6] + a2 * b[10] + a3 * b[14];
  tmp[7] = a0 * b[3] + a1 * b[7] + a2 * b[11] + a3 * b[15];

  a0 = a[8]; a1 = a[9]; a2 = a[10]; a3 = a[11];
  tmp[8] = a0 * b[0] + a1 * b[4] + a2 * b[8] + a3 * b[12];
  tmp[9] = a0 * b[1] + a1 * b[5] + a2 * b[9] + a3 * b[13];
  tmp[10] = a0 * b[2] + a1 * b[6] + a2 * b[10] + a3 * b[14];
  tmp[11] = a0 * b[3] + a1 * b[7] + a2 * b[11] + a3 * b[15];

  a0 = a[12]; a1 = a[13]; a2 = a[14]; a3 = a[15];
  tmp[12] = a0 * b[0] + a1 * b[4] + a2 * b[8] + a3 * b[12];
  tmp[13] = a0 * b[1] + a1 * b[5] + a2 * b[9] + a3 * b[13];
  tmp[14] = a0 * b[2] + a1 * b[6] + a2 * b[10] + a3 * b[14];
  tmp[15] = a0 * b[3] + a1 * b[7] + a2 * b[11] + a3 * b[15];
  memcpy(m, tmp, sizeof(GLfloat) * 4 * 4);
}

void LoadIdentity(GLfloat* m) {
  memset(m, 0, sizeof(GLfloat) * 4 * 4);
  m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void Transpose(GLfloat mOut[16], GLfloat mIn[16]) {
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
		mOut[(4 * i) + j] = mIn[(4 * j) +  i];
}

// Augment matrix, by adding identity matrix to the right
static void Augment(GLfloat mOut[32], GLfloat mIn[16]) {
  GLfloat mIdentity[16];
  LoadIdentity(mIdentity);
/*
  printf("\nOrig matrix\n");
  PrintMatrix(mIn, 4, 4);
*/ 
  memcpy(mOut, mIn, 16 * sizeof(GLfloat));
  memcpy(&mOut[16], mIdentity, 16 * sizeof(GLfloat));
}

// Make the matrix upper (right) triangular by Gaussian elimination.
// This method only adds multiples of rows to other rows. No rows are
// scaled up or switched, and the determinant is preserved.
// Ported from sylvester.src.js.
static void ToRightTriangular(GLfloat *mOut, GLfloat *mIn, int nRows, int nColumns) {
#define M(x, y) mOut[x + (nRows * y)]
  memcpy(mOut, mIn, nRows * nColumns * sizeof(GLfloat));
/*
  printf("\nAugmented matrix\n");
  PrintMatrix(mOut, 4, 8);
*/
  int n = nRows, k = n;
  int i, j, np, kp = nColumns, p;

  do { 
    i = k - n;

    // add elements of second row to first, initial element is 0
    if (M(i, i) == 0) {
      for (j = i + 1; j < k; j++) {
        if (M(j, i) != 0) {
          np = kp;

          do { 
            p = kp - np;
            M(i, p) = M(i, p) + M(j, p);
          } while (--np);

          break;
        }
      }
    }

    if (M(i,i) != 0) {
      for (j = i + 1; j < k; j++) {
        GLfloat multiplier = M(j,i) / M(i,i);
        np = kp;

         do { 
           p = kp - np;

           // Elements with column numbers up to an including the number
           // of the row that we're subtracting can safely be set straight to
           // zero, since that's the point of this routine and it avoids having
           // to loop over and correct rounding errors later
           M(j, p) = (p <= i ? 0 : M(j, p) - M(i, p) * multiplier);
//           printf("Index: %d ---- %f = %f - %f * %f\n", p, row[p], M(j,p), M(i,p), multiplier);
         } while (--np);
       }
     }
  } while (--n);
#undef M
}

// Returns the inverse (if one exists) using Gauss-Jordan
// Ported from sylvester.src.js.
void Inverse(GLfloat mOut[16], GLfloat mIn[16]) {
#define M(x, y) mInverse[x + (4 * y)]
#define O(x, y) mOut[x + (4 * y)]

  GLfloat mAug[32], mInverse[32];
  int ni = 4;
  int ki = ni, i, j;
  int np, kp = 8, p;
  GLfloat divisor;
  GLfloat row[kp];

  memset(mOut, 0, 16 * sizeof(GLfloat));

// ignore singular checks for now...
//	if (!this.isSquare() || this.isSingular()) { return null; }

  // augment original Matrix with identity matrix
  Augment(mAug, mIn);
  // and turn result into a right triangular matrix
  ToRightTriangular(mInverse, mAug, 4, 8);

	// Matrix is non-singular so there will be no zeros on the diagonal
	// Cycle through rows from last to first
	do 
	{ 
		i = ni - 1;
		// First, normalise diagonal elements to 1
		np = kp;
		divisor = M(i, i);

		do 
		{ 
			p = kp - np;
			M(i, p) = M(i, p) / divisor;
			// Shuffle of the current row of the right hand side into the results
			// array as it will not be modified by later runs through this loop

			if (p >= ki)
				O(i, (p - 4)) = M(i, p);

//			printf("p: %d, i: %d, %.4f\n", p, i, M(i, p));
		} 
		while (--np);

		// Then, subtract this row from those above it to
		// give the identity matrix on the left hand side
		for (j = 0; j < i; j++) 
		{
			np = kp;
			do 
			{ 
				p = kp - np;
				row[p] = M(j, p) - M(i, p) * M(j, i);
			} 
			while (--np);

			// update working matrix
			for (p = 0; p < kp; p++)
				M(j, p) = row[p];
		}
	} 
	while (--ni > 0);

#undef M
#undef O
}

}  // namespace transform_4x4
