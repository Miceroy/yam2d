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
#ifndef ANIMATEDENEMY_H_
#define ANIMATEDENEMY_H_ 

// Include base class
#include <SpriteGameObject.h>
#include <AnimationTrack.h>

// Forward declaration of class Player
class Player;

// Use SpriteGameObject as base class for our player, 
// because player have non animated sprite. SpriteGameObject provides 
// "automatic rendering of sprite" to corrent position on map.
class AnimatedEnemy : public yam2d::SpriteGameObject
{
public:
	/** Constructor of player. 
	 *
	 * @param gameObjectType Game specific game object type. Useful for for example detecting of "real game object type", like Player or Enemy.
	 * @param texture Texture for our game object.
	 */
	AnimatedEnemy(int gameObjectType, yam2d::Texture* texture, const yam2d::vec2& initialPosition, Player* player);
	virtual ~AnimatedEnemy(void);

	// This virtual method is automatically called byt map/layer, when update is called from main.cpp
	virtual void update( float deltaTime );

	
private:
//	std::vector<yam2d::vec2> m_waypoints;
	Player* m_player;

	float m_totalTime;

	yam2d::Ref<yam2d::AnimationTimeline> m_timeline;

	yam2d::Ref<yam2d::AnimationTrack<yam2d::vec2, AnimatedEnemy> > m_positionAnimationTrack; 
	yam2d::Ref<yam2d::AnimationTrack<float, AnimatedEnemy> > m_rotationAnimationTrack; 
};


#endif