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
#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <Object.h>
#include <vector>

namespace yam2d
{
	
class Texture;
class SpriteSheet;

/**
 * Class for SpriteAnimation.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class SpriteAnimation : public Object
{
public:
	struct SpriteAnimationClip
	{
		SpriteAnimationClip()
			: indices(0)
			, sizeOfIndices(0)
			, animationFPS(0)
			, xScale(1.0f)
		{
		}

		SpriteAnimationClip( const int* indices, int sizeOfIndices, float animationFPS, float xScale, bool loop )
			: indices(indices)
			, sizeOfIndices(sizeOfIndices)
			, animationFPS(animationFPS)
			, xScale(xScale)
			, loop(loop)
		{
		}

		const int*	indices;
		int			sizeOfIndices;
		float		animationFPS;
		float		xScale;
		bool		loop;
	};

	SpriteAnimation();

	virtual ~SpriteAnimation() {}

	void update( float deltaTime );

	int getCurrentClipIndex();

	float getXScale()
	{
		return m_clips[m_animationId].xScale;
	}

	void addAnimation( int animationId, const SpriteAnimationClip& clip )
	{
		if( m_clips.size() <= size_t(animationId) )
		{
			m_clips.resize(animationId+1);
		}

		m_clips[animationId] = clip;
	}

	void setActiveAnimation(int animationId)
	{
		m_totalTime = 0.0f;
		m_animationId = animationId;
	}

	int getActiveAnimation()
	{
		return m_animationId;
	}

private:
	std::vector<SpriteAnimationClip>	m_clips;
	int									m_animationId;
	float								m_totalTime;

	// Hidden
	SpriteAnimation(const SpriteAnimation&);
	SpriteAnimation& operator=(const SpriteAnimation&);
};


}

#endif // SPRITEANIMATION_H

