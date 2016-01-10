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
#ifndef ANIMATEDSPRITEGAMEOBJECT_H_
#define ANIMATEDSPRITEGAMEOBJECT_H_

#include <vector>
#include <string>
#include <map>

#include <SpriteSheetComponent.h>

namespace yam2d
{
	

class Tileset;
class Map;
class SpriteSheet;


/**
 * Class for AnimatedSpriteGameObject.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class AnimatedSpriteComponent : public SpriteSheetComponent, public Updatable
{
public:
	// Default constructor.
	AnimatedSpriteComponent(GameObject* owner, SpriteSheet* spriteSheet)
		: SpriteSheetComponent(owner, spriteSheet, 0)
		, m_animation(new SpriteAnimation())
	{
	}

	virtual ~AnimatedSpriteComponent() {}

	void addAnimation( int animationId, const SpriteAnimation::SpriteAnimationClip& clip )
	{
		m_animation->addAnimation(animationId,clip);
	}

	void setActiveAnimation(int animationId)
	{
		m_animation->setActiveAnimation(animationId);
	}

	int getActiveAnimation()
	{
		return m_animation->getActiveAnimation();
	}

	bool isFinished()
	{
		return m_animation->isFinished();
	}

	int getNumAnimations()
	{
		return m_animation->getNumAnimations();
	}
	
	virtual void update(float deltaTime)
	{
		m_animation->update(deltaTime);
	}
	
	SpriteAnimation* getAnimation() const { return m_animation.ptr(); }
private:
	Ref<SpriteAnimation> m_animation;

	AnimatedSpriteComponent();
	AnimatedSpriteComponent(const AnimatedSpriteComponent& o);
	AnimatedSpriteComponent& operator=(const AnimatedSpriteComponent& o);
};


}

#endif




