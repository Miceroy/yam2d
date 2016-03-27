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
#include <cstddef>
#include <SpriteSheet.h>

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
			: indices()
			, animationFPS(0)
			, xScale(1.0f)
		{
		}

		SpriteAnimationClip(SpriteSheet* spriteSheet, float animationFPS, float xScale, bool loop)
			: indices(generateIndicesFromSpriteSheet(spriteSheet))
			, animationFPS(animationFPS)
			, xScale(xScale)
			, loop(loop)
		{
			assert(indices.size() > 0); // No indices!!
		}


		SpriteAnimationClip(const std::vector<int>& ind, float animationFPS, float xScale, bool loop)
			: indices(ind)
			, animationFPS(animationFPS)
			, xScale(xScale)
			, loop(loop)
		{
			assert(indices.size() > 0); // No indices!!
		}

		std::vector<int> indices;
		float		animationFPS;
		float		xScale;
		bool		loop;
	};

	SpriteAnimation();

	virtual ~SpriteAnimation() {}

	void update(float deltaTime);

	int getCurrentClipIndex();

	float getXScale()
	{
		return m_clips[m_animationId].xScale;
	}

	void addAnimation(int animationId, const SpriteAnimationClip& clip)
	{
		assert(clip.indices.size() > 0); // No indices!!
		if (m_clips.size() <= size_t(animationId))
		{
			m_clips.resize(animationId + 1);
		}

		m_clips[animationId] = clip;
	}

	void setActiveAnimation(int animationId)
	{
		if (animationId != m_animationId)
		{
			m_totalTime = 0.0f;
			m_animationId = animationId;
			m_isFinished = false;
		}
	}

	int getActiveAnimation()
	{
		return m_animationId;
	}

	int getNumAnimations()
	{
		return m_clips.size();
	}

	bool isFinished()
	{
		return m_isFinished;
	}

private:
	std::vector<SpriteAnimationClip>	m_clips;
	int									m_animationId;
	float								m_totalTime;
	bool								m_isFinished;

	static std::vector<int> generateIndicesFromSpriteSheet(SpriteSheet* spriteSheet)
	{
		std::vector<int> indices;
		assert(spriteSheet->getClipCount() > 0);
		indices.resize(spriteSheet->getClipCount());
		for (size_t i = 0; i < indices.size(); ++i)
		{
			indices[i] = i;
		}

		return indices;
	}

	// Hidden
	SpriteAnimation(const SpriteAnimation&);
	SpriteAnimation& operator=(const SpriteAnimation&);
};


}

#endif // SPRITEANIMATION_H

