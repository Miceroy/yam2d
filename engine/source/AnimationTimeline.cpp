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
#include <AnimationTimeline.h>

namespace yam2d
{
		//enum RepeatMode
		//{
		//	CONSTANT	= 1, // Plays only once
		//	LOOP		= -1  // Looping mode			
		//};

AnimationTimeline::AnimationTimeline(float maxTime)
: Object()
, m_repeatMode(CONSTANT)
, m_maxTime(maxTime)
, m_speed(1.0f)
, m_currentTime(0.0f)
{
	assert( m_maxTime > 0.0f );
}


AnimationTimeline::~AnimationTimeline()
{
}


// Sets speed of the animation. 0 = pause, 1 = forward, -1 = reverse, 2 = double speed etc...
void AnimationTimeline::setSpeed( float speed )
{
	m_speed = speed;
}

// Sets current time in animation
void AnimationTimeline::setCurrentTime( float currentTime )
{
	if( m_repeatMode == CONSTANT )
	{
	//	assert( currentTime >= 0 && currentTime <= m_maxTime );

		// m_currentTime = clamp(0,m_maxTime,currentTime)
		if( currentTime > m_maxTime )
			m_currentTime = m_maxTime;
		else if ( currentTime < 0.0f )
			m_currentTime = 0.0f;
		else
			m_currentTime = currentTime; 
	}
	else
	{
		if( currentTime > 0.0f )
		{
			// Scale time
			float scaledCurrentTime = currentTime / m_maxTime;
		
			// Take only fractional part
			float fractional = scaledCurrentTime - float(int(scaledCurrentTime));
		
			// Scale fractional to max time.
			m_currentTime = fractional * m_maxTime;
		}
		else
		{
			// Scale time
			float scaledCurrentTime = -currentTime / m_maxTime;
		
			// Take only fractional part
			float fractional = scaledCurrentTime - float(int(scaledCurrentTime));
		
			// Scale fractional to max time.
			m_currentTime = m_maxTime - (fractional * m_maxTime);
		}
	}

//	m_currentTime = currentTime;
}
	
// void sets repeat mode
void AnimationTimeline::setRepeatMode(int repeatMode)
{
	assert( repeatMode == LOOP || repeatMode == CONSTANT );
	m_repeatMode = (RepeatMode)repeatMode;
}


// Call this on update. Progresses animation according to given delta time and speed.
void AnimationTimeline::update( float deltaTime )
{
	setCurrentTime( m_currentTime + deltaTime * m_speed );	
}


// Returns scaled time of animation. Dependent on speed etc.
float AnimationTimeline::getCurrentTime() const
{
	return m_currentTime;
}


// Returns maximum time of the animation, if animation is in CONSTANT-repeat mode. This method must not be called in LOOP-repeat mode.
float AnimationTimeline::getMaxTime() const
{
	assert( m_repeatMode == CONSTANT );
	return m_maxTime;
}


// Returns true, if animation has ended (current time > m_maxTime) in CONSTANT-repeat mode. In case of LOOP-repeat mode, returns false always.
bool AnimationTimeline::hasEnded() const
{
	return (m_repeatMode == CONSTANT) ? (getCurrentTime() > getMaxTime()) : false;
}


}
