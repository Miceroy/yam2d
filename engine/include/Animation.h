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
#ifndef ANIMATION_H_
#define ANIMATION_H_


#include <Object.h>
#include <Ref.h>
#include <vector>
#include <utility>

namespace yam2d
{


	template<class T>
	struct Keyframe
	{
	public:
		inline Keyframe(const T& v, float t);

		T		value;
		float	time;
	private:
		Keyframe();
	};

	// Linear/slerp/step
	template<class T>
	class KeyframeSequence : public Object
	{
	public:
		typedef T (*InterpolateFuncType)(const T& v1, const T& v2, float interpolant);

		template<class ValueType>
		static ValueType LERP(const ValueType& v1, const ValueType& v2, float interpolant)
		{
			return (1.0f-interpolant)*v1 + interpolant*v2;
		}

		template<class ValueType>
		static ValueType STEP(const ValueType& v1, const ValueType& v2, float interpolant)
		{
			return v1;
		}

		inline KeyframeSequence(const std::vector< Keyframe<T> >& keyframes);

		inline KeyframeSequence(const Keyframe<T>* const data, size_t len);
		
		virtual ~KeyframeSequence();

		inline void setInterpolation(InterpolateFuncType func) { m_interpolate = func; }
		inline T getValue(float time) const;
		
		inline float getDuration() const;

	private:
		std::vector<Keyframe<T> >	m_keyframes;
		InterpolateFuncType			m_interpolate;

		inline void setKeyframes(const Keyframe<T>* const data, size_t len);

		KeyframeSequence();
		KeyframeSequence(const KeyframeSequence&);
		KeyframeSequence& operator=(const KeyframeSequence&);
	};


	class AnimationTimeline : public Object
	{
	public:
		enum RepeatMode
		{
			CONSTANT	= 1, // Plays only once
			LOOP		= -1  // Looping mode			
		};

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

	template<class ValueType, class TargetObjectType>
	class AnimationTrack : public Object
	{
	public:
		typedef void (TargetObjectType::*TargetValueSetterMethodType1)(const ValueType&);
		typedef void (TargetObjectType::*TargetValueSetterMethodType2)(ValueType);

		inline AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType1 targetProperty);
		inline AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType2 targetProperty);
		
		virtual ~AnimationTrack();

		inline void addKeyframes( float weight, KeyframeSequence<ValueType>* keyframes );

		inline void applyValues();

	private:
		typedef std::pair<float, Ref<KeyframeSequence<ValueType> > > KeyframeWeight;

		AnimationTimeline*				m_timeline;
		std::vector<KeyframeWeight>		m_keyframes;
		TargetObjectType*				m_targetObject;
		TargetValueSetterMethodType1	m_targetProperty1;
		TargetValueSetterMethodType2	m_targetProperty2;

		AnimationTrack();
		AnimationTrack(const AnimationTrack&);
		AnimationTrack& operator=(const AnimationTrack&);
	};

}

#include <Animation.inl>

#endif

