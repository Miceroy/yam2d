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

#include "SpriteAnimation.h"
#include "SpriteSheet.h"
#include "Texture.h"


namespace yam2d
{

SpriteAnimation::SpriteAnimation()
	: m_animationId(-1)
	, m_totalTime(0)
{
}


void SpriteAnimation::update( float deltaTime )
{
	m_totalTime += deltaTime;
}


int SpriteAnimation::getCurrentClipIndex()
{
	if( m_animationId < 0 )
		return -1;

	int index = int(m_totalTime * m_clips[m_animationId].animationFPS);

	if( m_clips[m_animationId].loop )
	{
		index %= m_clips[m_animationId].sizeOfIndices;
	}
	else
	{
		if( index >= m_clips[m_animationId].sizeOfIndices )
		{
			index = m_clips[m_animationId].sizeOfIndices-1;
		}
	}

	int clipIndex = m_clips[m_animationId].indices[index];
	return clipIndex;
}

}
