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
#ifndef ANIMATIONTRACK_H_
#define ANIMATIONTRACK_H_



#include <KeyframeSequence.h>
#include <AnimationTimeline.h>

namespace yam2d
{
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

#include <AnimationTrack.inl>

#endif

