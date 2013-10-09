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

	/**
	 * Keyframe Declares value to certain time in key frame sequence.
	 */
	template<class T>
	struct Keyframe
	{
	public:
		/**
		 * Constructor. Declares value time pair, which tells what the value should be in wich time on animation.
		 */
		inline Keyframe(const T& v, float t);

		T		value;
		float	time;
	private:
		Keyframe();
	};

	/**
	 * Keyframe sequence contains array of time value pairs (keyframes).
	 *
	 * If you need to use same keyframedata in several animation, it is recommended, that you share
	 * same KeyframeSequence-object between animations instead of keeping same data twice in the memory.  
	 */
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

		template<class ValueType>
		static ValueType SLERP(const ValueType& v1, const ValueType& v2, float interpolant)
		{
			return slerp(v1, v2, interpolant);
		}

		/**
		 * Constructs KeyframeSequence from std::vector<Keyframe>. 
		 */
		inline KeyframeSequence(const std::vector< Keyframe<T> >& keyframes);

		/**
		 * Constructs KeyframeSequence from c-style array. Copies data to it's internal datastructure, so after construction,
		 * data allocated by caller can be free'ed.
		 */
		inline KeyframeSequence(const Keyframe<T>* const data, size_t len);
		
		virtual ~KeyframeSequence();

		/**
		 * Sets interpolation function used bu this key frame sequence. Can 
		 * be KeyframeSequence::LERP, KeyframeSequence::STEP or KeyframeSequence::SLERP.
		 * You can also provide your own interpolation function type if function is in 
		 * correct format.
		 */
		inline void setInterpolation(InterpolateFuncType func) { m_interpolate = func; }

		/**
		 * Returns interpolated value of keyframe sequence in given time. Time must be in range: 0 <= time <= getDuration()
		 */
		inline T getValue(float time) const;
		
		/**
		 * Returns duration of this KeyframeSequence.
		 */
		inline float getDuration() const;

	private:
		std::vector<Keyframe<T> >	m_keyframes;
		InterpolateFuncType			m_interpolate;

		inline void setKeyframes(const Keyframe<T>* const data, size_t len);

		KeyframeSequence();
		KeyframeSequence(const KeyframeSequence&);
		KeyframeSequence& operator=(const KeyframeSequence&);
	};

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

	/**
	 * AnimationTrack composes several matters together inorder to automate animation time mapping to some target property of an object.
	 * 
	 * AnimationTrack has always some (game)object attached to it. Each animation track controls certain target property of the object, 
	 * like position, rotation, transparency etc. vie targetProperty-setter function.
	 *
	 * There can be serveral Keyframes, which is used to control the target property of an object. These single keyframe values are take
	 * into account, when final target value is calculated. Actual final target value is calculated as weighted sum of each value of keyframe.
	 */
	template<class ValueType, class TargetObjectType>
	class AnimationTrack : public Object
	{
	public:
		typedef void (TargetObjectType::*TargetValueSetterMethodType1)(const ValueType&);
		typedef void (TargetObjectType::*TargetValueSetterMethodType2)(ValueType);

		inline AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType1 targetProperty);
		inline AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType2 targetProperty);
		
		virtual ~AnimationTrack();

		/**
		 * Adds new keyframe sequence with specifiec blend weight.
		 */
		inline void addKeyframes( float weight, KeyframeSequence<ValueType>* keyframes );

		/**
		 * Calculates each keyframe value from KeyframeSequences according to time of AnimationTimeline object. 
		 * Calculates weighted sum of each keyframe values and set it to given target object using tergetProperty-method pointer.
		 */
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

