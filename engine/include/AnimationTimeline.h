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
#ifndef ANIMATIONTIMELINE_H_
#define ANIMATIONTIMELINE_H_

#include <Object.h>

namespace yam2d
{
	/**
	 * AnimationTimeline keeps track of time of the animation. Depending on the application, there might have several AnimationTimeline-objects.
	 * AnimationTimeline object is used to control timing of AnimationTrack and in case of shared AnimationTimeline between AnimationTracks,
	 * those AnimationTracks run in same time.
	 */
	class AnimationTimeline : public Object
	{
	public:
		enum RepeatMode
		{
			CONSTANT	= 1, // Plays only once
			LOOP		= -1  // Looping mode			
		};

		/** Constructs AnimationTimeline with maximum time. */
		AnimationTimeline(float maxTime);

		virtual ~AnimationTimeline();

		// Sets speed of the animation. 0 = pause, 1 = forward, -1 = reverse, 2 = double speed etc...
		void setSpeed(float speed);
		
		// Sets current time in animation
		void setCurrentTime(float currentTime);
	
		// void sets repeat mode
		void setRepeatMode(int repeatMode);

		// Call this on update. Progresses animation according to given delta time and speed.
		void update(float deltaTime);

		// Returns scaled time of animation. Dependent on speed etc.
		float getCurrentTime() const;

		// Returns maximum time of the animation, if animation is in CONSTANT-repeat mode. This method must not be called in LOOP-repeat mode.
		float getMaxTime() const;

		// Returns true, if animation has ended (current time > maxTime) in CONSTANT-repeat mode. In case of LOOP-repeat mode, returns false always.
		bool hasEnded() const;

	private:
		RepeatMode		m_repeatMode;
		float			m_maxTime;
		float			m_speed;
		float			m_currentTime;

		AnimationTimeline();
		AnimationTimeline(const AnimationTimeline&);
		AnimationTimeline& operator=(const AnimationTimeline&);
	};


}


#endif

