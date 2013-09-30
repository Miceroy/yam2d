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

namespace yam2d
{

template<class T>
Keyframe<T>::Keyframe(const T& v, float t)
	: value(v)
	, time(t)
{
}


template<class T>
KeyframeSequence<T>::KeyframeSequence(const std::vector<Keyframe<T> >& keyframes)
	: m_interpolate(LERP)
{
	setKeyframes(&keyframes[0],keyframes.size());
}


template<class T>
KeyframeSequence<T>::KeyframeSequence(const Keyframe<T>* const data, size_t len)
	: m_interpolate(LERP)
{
	setKeyframes(data,len);
}

template<class T>
KeyframeSequence<T>::~KeyframeSequence()
{
}


template<class T>
T KeyframeSequence<T>::getValue(float time) const
{
	assert( time >= 0.0f );	assert( time <= getDuration() );

	size_t i=0;
	while( time >= m_keyframes[i].time )
	{
		if( time == m_keyframes[i].time )
			return m_keyframes[i].value;

		++i;
	}
	
	// i contains now index of upper value
	assert( i >= 1 ); assert( i < m_keyframes.size() );

	int k1 = int(i)-1;
	int k2 = i;
		
	float interpolant = (time-m_keyframes[k1].time) / (m_keyframes[k2].time-m_keyframes[k1].time);
	assert( interpolant >= 0.0f ); assert( interpolant <= 1.0f );
	return m_interpolate(m_keyframes[k1].value, m_keyframes[k2].value, interpolant);
}

template<class T>
float KeyframeSequence<T>::getDuration() const
{
	return m_keyframes[m_keyframes.size()-1].time;
}


template<class T>
void KeyframeSequence<T>::setKeyframes(const Keyframe<T>* const data, size_t len)
{
	assert(len >= 2); // Must have atleast 2 keyframes
	assert(data != 0);
	// Check that kf:s are in correct order
	{
		float prevT = data[0].time;
		assert(prevT == 0.0f); // First time value must be 0
		for( size_t i=1; i<len; ++i )
		{
			assert( prevT < data[i].time ); // Times must be in growing order.
			prevT = data[i].time;
		}
	}

	m_keyframes.reserve(len);
	for( size_t i=0; i<len; ++i )
	{
		m_keyframes.push_back(data[i]);
	}
}

template<class ValueType, class TargetObjectType>
AnimationTrack<ValueType,TargetObjectType>::AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType1 targetProperty)
	: Object()
	, m_timeline(timeline)
	, m_keyframes()
	, m_targetObject(targetObject)
	, m_targetProperty1(targetProperty)
	, m_targetProperty2(0)
{
}

template<class ValueType, class TargetObjectType>
AnimationTrack<ValueType,TargetObjectType>::AnimationTrack(AnimationTimeline* timeline, TargetObjectType* targetObject, TargetValueSetterMethodType2 targetProperty)
	: Object()
	, m_timeline(timeline)
	, m_keyframes()
	, m_targetObject(targetObject)
	, m_targetProperty1(0)
	, m_targetProperty2(targetProperty)
{
}

template<class ValueType, class TargetObjectType>
AnimationTrack<ValueType,TargetObjectType>::~AnimationTrack()
{
}

template<class ValueType, class TargetObjectType>
void AnimationTrack<ValueType,TargetObjectType>::addKeyframes( float weight, KeyframeSequence<ValueType>* keyframes )
{
	m_keyframes.push_back( KeyframeWeight(weight,keyframes) );
}

template<class ValueType, class TargetObjectType>
void AnimationTrack<ValueType,TargetObjectType>::applyValues()
{
	//()setPosition( m_kf->getValue( m_timeline->getCurrentTime() ) );
	assert( m_keyframes.size() > 0 ); // Atleast one kf sequence required.
	float curTime = m_timeline->getCurrentTime();
	ValueType val = m_keyframes[0].first * m_keyframes[0].second->getValue(curTime);

	for( size_t i=1; i< m_keyframes.size(); ++i )
	{
		val += m_keyframes[i].first * m_keyframes[i].second->getValue(curTime);
	}

	if( m_targetProperty1 != 0 )
	{
		(m_targetObject->*m_targetProperty1)(val);
	}

	if( m_targetProperty2 != 0 )
	{
		(m_targetObject->*m_targetProperty2)(val);
	}
}




}
