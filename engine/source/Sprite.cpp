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

#include "Sprite.h"
//#include <string.h>
#include "es_assert.h"


namespace yam2d
{


Sprite::Sprite()
: m_scale(1.0f)
, m_cropStart(0.0f)
, m_cropSize(1.0f)
, m_depth(0.0f)
{
	setColor(1.0f,1.0f,1.0f,1.0f);
}


Sprite::~Sprite()
{
}


void Sprite::setScale( const vec2& scale )
{
	m_scale = scale;
}


void Sprite::setDepth( float depth )
{
	m_depth = depth;
}


void Sprite::setOpacity( float a )
{
	m_color[3] = a;
}


void Sprite::setColor( float r, float g, float b, float a )
{
	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;
	m_color[3] = a;
}


void Sprite::setClip(const vec2& start, const vec2& size)
{
	assert( (start.x >= 0.0) && (start.x <= 1.0f) );
	assert( (start.y >= 0.0) && (start.y <= 1.0f) );

	assert( (size.x >= 0.0) && (size.x <= 1.0f) );
	assert( (size.y >= 0.0) && (size.y <= 1.0f) );

	assert( ((size.x+start.x) >= 0.0) && ((size.x+start.x) <= 1.01f) );
	assert( ((size.y+start.y) >= 0.0) && ((size.y+start.y) <= 1.01f) );
	
	m_cropSize = size;
	m_cropStart = start;
}


const vec2& Sprite::getScale() const
{
	return m_scale;
}


void Sprite::getVertexData( std::vector<float>& verts, std::vector<float>& texCoords, std::vector<float>& colors ) const
{
	{
		size_t startIndex = verts.size();
		static const float S = 0.5f;
		static const size_t DS = 3*6;
		float v [DS] = {
			-S*m_scale.x,		-S*m_scale.y,		m_depth,
			 S*m_scale.x,		 S*m_scale.y,		m_depth,
			-S*m_scale.x,		 S*m_scale.y,		m_depth,
			-S*m_scale.x,		-S*m_scale.y,		m_depth,
			 S*m_scale.x,		-S*m_scale.y,		m_depth,
			 S*m_scale.x,		 S*m_scale.y,		m_depth
		};

		verts.resize( verts.size()+DS );
		memcpy(&verts[startIndex], &v, sizeof(v) );
	}

	{
		size_t startIndex = texCoords.size();
		static const size_t DS = 2*6;
		float v [DS] = {
			m_cropStart.x,					(m_cropStart.y+m_cropSize.y),
			m_cropStart.x+m_cropSize.x,		(m_cropStart.y),
			m_cropStart.x,					(m_cropStart.y),
			m_cropStart.x,					(m_cropStart.y+m_cropSize.y),
			m_cropStart.x+m_cropSize.x,		(m_cropStart.y+m_cropSize.y),
			m_cropStart.x+m_cropSize.x,		(m_cropStart.y),
		};


		texCoords.resize( texCoords.size()+DS );
		memcpy(&texCoords[startIndex], &v, sizeof(v) );
	}

	for( int i=0; i<6; ++i )
	{
		size_t startIndex = colors.size();
		colors.resize( colors.size()+4 );
		memcpy(&colors[startIndex], &m_color, sizeof(m_color) );
	}
}


void Sprite::setClip( float textureWidth, float textureHeight, PixelClip clip )
{
	textureWidth = 1.0f/textureWidth;
	textureHeight = 1.0f/textureHeight;

	float TOL = 0.1f;
	vec2 topLeftInPixels(0);
	topLeftInPixels.x = float(clip.topLeft.x)+TOL;
	topLeftInPixels.y = float(clip.topLeft.y)+TOL;
	vec2 clipSizeInPixels(0);
	clipSizeInPixels.x = float(clip.clipSize.x)-TOL;
	clipSizeInPixels.y = float(clip.clipSize.y)-TOL;
	vec2 topLeftInScreen = topLeftInPixels;
	topLeftInScreen.x *= textureWidth;
	topLeftInScreen.y *= textureHeight;

	vec2 clipSizeInScreen = clipSizeInPixels;
	clipSizeInScreen.x *= textureWidth;
	clipSizeInScreen.y *= textureHeight;

	clipSizeInPixels.x += 2.0f*TOL;
	clipSizeInPixels.y += 2.0f*TOL;
	setScale(clipSizeInPixels);
	setClip(topLeftInScreen, clipSizeInScreen);
}

}
