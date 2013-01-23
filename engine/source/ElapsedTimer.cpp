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
#include <config.h>

namespace yam2d
{

// anonymous namespace for internal functions
namespace
{
#if defined(_WIN32)
	#include <Windows.h>

#if defined(ELAPSED_TIMER_USES_GETTICCOUNT)
	inline  __int64 getTimeScale()
	{
		return 1000;
	}

	/** Returns time in seconds */
	inline __int64 getTotalTime()
	{
		return GetTickCount();
	}
#else
	static LARGE_INTEGER pcFreq;
	static BOOL pcAvailable = QueryPerformanceFrequency(&pcFreq);

	inline  __int64 getTimeScale()
	{
		if( pcAvailable )
		{
			return pcFreq.QuadPart;
		}

		return 1000;
	}

	/** Returns time in seconds */
	inline __int64 getTotalTime()
	{
		LARGE_INTEGER curTime;

		if( pcAvailable && QueryPerformanceCounter(&curTime) )
		{
			return curTime.QuadPart;
		}
		else
		{
			return GetTickCount();
		}
	}
#endif

#else
You need to have unsigned long getTime() implementation on this platform.
#endif
}


ElapsedTimer::ElapsedTimer()
: m_startTime((-1))
{
}


ElapsedTimer::~ElapsedTimer()
{
}


void ElapsedTimer::reset()
{
	m_startTime = getTotalTime();
}


float ElapsedTimer::getTime() const
{
	assert(m_startTime != __int64(-1) ); // You must call reset atleast once before first call to getTime.
	__int64 curTime = getTotalTime();

	float deltaTime = float(curTime-m_startTime)/float(getTimeScale());
	assert( deltaTime >= 0.0f ); // WTF? 
	return deltaTime;
}	

}



