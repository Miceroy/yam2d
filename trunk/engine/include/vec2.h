// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// http://code.google.com/p/yam2d/
//
// Copyright (c) 2013 Mikko Romppainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef VEC2_H_
#define VEC2_H_

//#include <Box2D/Common/b2Math.h>
#include <slm\slmath.h>

namespace yam2d
{

typedef	slm::vec2 vec2;

class vec2int
{
public:
	vec2int(int x0, int y0)
	: x(x0)
	, y(y0)
	{
	}

	vec2int(int v=0)
	: x(v)
	, y(v)
	{
	}


	int x;
	int y;
};

/**
 * Rotates given vector according to given angle.
 */
inline vec2 rotateVector(const vec2& vec, float angle )
{
	vec2 res;
	// Rotate vector according to angle (see http://en.wikipedia.org/wiki/Rotation_(mathematics))
	float sinAngle = sinf(angle);
	float cosAngle = cosf(angle);
	res.x = vec.x*cosAngle - vec.y*sinAngle;
	res.y = vec.x*sinAngle + vec.y*cosAngle;
	return res;
}

}

#endif

