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

#include "ElapsedTimer.h"	
#include "es_assert.h"


namespace yam2d
{

// anonymous namespace for internal functions
namespace
{
#if defined(_WIN32)
	#include <Windows.h>
	
	inline unsigned long getTicks()
	{
		return GetTickCount();
	}
#else
You need to have unsigned long getTicks() implementation on this platform.
#endif
}


ElapsedTimer::ElapsedTimer()
: m_startTime(unsigned long(-1))
{
}


ElapsedTimer::~ElapsedTimer()
{
}


void ElapsedTimer::reset()
{
	m_startTime = GetTickCount();
}


float ElapsedTimer::getTime() const
{
	assert(m_startTime != unsigned long(-1) ); // You must call reset atleast once before firs call to getTime.
	unsigned long curTime = GetTickCount();
	assert( curTime >= m_startTime ); // WTF? Overflow
	return float(curTime-m_startTime)*0.001f;
}	

}



