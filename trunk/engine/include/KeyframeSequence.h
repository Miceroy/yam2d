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
#ifndef KEYFRAMESEQUENCE_H_
#define KEYFRAMESEQUENCE_H_


#include <Object.h>
#include <Ref.h>
#include <vector>
#include <Keyframe.h>

namespace yam2d
{
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


}

#include <KeyframeSequence.inl>

#endif

