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
#include "AnimatedEnemy.h" // Include Enemy class header

#include "Input.h"
#include "Player.h"
#include "es_util.h"

using namespace yam2d; // Use namespace yam3d implicitily.



AnimatedEnemy::AnimatedEnemy(int gameObjectType, Texture* texture, const vec2& initialPosition, Player* player)
	: SpriteGameObject(gameObjectType,texture) // Initalize base class by giving parameres to it
	, m_player(player)
{
	
	// Create and configure animation timeline. This can also be in application main class instead on object.
	m_timeline = new AnimationTimeline(4.0f); // Duration 4 seconds
	m_timeline->setRepeatMode(AnimationTimeline::LOOP); // Looping animation

	// Create animation track for controlling this object. Map timeline and keyframes in way, that call 
	// to AnimationTrack::applyValues, causes call to AnimatedEnemy::setPosition, using correctly animated value.
	m_positionAnimationTrack = new AnimationTrack<vec2,AnimatedEnemy>(m_timeline, this, &AnimatedEnemy::setPosition);

	{
		// Position keyframe data. This is position, where to be in which time in timeline
		Keyframe<vec2> positionData[] = 
		{
			Keyframe<vec2>( vec2(0,0),		0.0f ), 
			Keyframe<vec2>( vec2(10,0),		1.0f ),
			Keyframe<vec2>( vec2(10,10),	2.0f ),
			Keyframe<vec2>( vec2(0,10),		3.0f ),
			Keyframe<vec2>( vec2(0,0),		4.0f )
		};
	
		// Create keyframe sequence from position data array.
		KeyframeSequence<vec2>* positionKeyframes = new KeyframeSequence<vec2>(&positionData[0],sizeof(positionData)/sizeof(positionData[0]));
		
		// Add keyframes using 1.0 blend weight
		m_positionAnimationTrack->addKeyframes( 1.0f, positionKeyframes );
	}
	
#if 1
	{ // This is "offset animation", this is blended additively (weight=1.0) on top of position animation.
		Keyframe<vec2> positionOffsetData[] = 
		{
			Keyframe<vec2>( initialPosition,		0.0f ), 
			Keyframe<vec2>( initialPosition,		4.0f )
		};
	
		// Create keyframe sequence from position data array.
		KeyframeSequence<vec2>* positionKeyframes = new KeyframeSequence<vec2>(&positionOffsetData[0],sizeof(positionOffsetData)/sizeof(positionOffsetData[0]));
		m_positionAnimationTrack->addKeyframes( 1.0f, positionKeyframes );
	}
#endif

	// Do same for rotation animation, which if type float (angle in radians).
	float PI = 3.14159f;
	
	Keyframe<float> rotationData[] = 
	{
		Keyframe<float>( 0,				0.0f ),
		Keyframe<float>( PI/2.0f,		1.0f ),
		Keyframe<float>( PI,			2.0f ),
		Keyframe<float>( -PI/2.0f,		3.0f ),
		Keyframe<float>( 0,				4.0f )
	};

	KeyframeSequence<float>* rotationKeyframes = new KeyframeSequence<float>(&rotationData[0],sizeof(rotationData)/sizeof(rotationData[0]));
	// Set step interpolation (rotate immediately)
	rotationKeyframes->setInterpolation( KeyframeSequence<float>::STEP );
	
	m_rotationAnimationTrack = new AnimationTrack<float, AnimatedEnemy>(m_timeline, this, &AnimatedEnemy::setRotation);
	m_rotationAnimationTrack->addKeyframes( 1.0f, rotationKeyframes );
}


AnimatedEnemy::~AnimatedEnemy(void)
{
}

void AnimatedEnemy::update( float deltaTime )
{
	// If collides to player, pause animation
	if( collidesTo(m_player) )
		m_timeline->setSpeed(0.0f);
	else
		m_timeline->setSpeed(1.0f);

	// Update timeline
	m_timeline->update(deltaTime);

	// Apply animation values
	m_positionAnimationTrack->applyValues();
	m_rotationAnimationTrack->applyValues();
}

