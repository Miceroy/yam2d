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
