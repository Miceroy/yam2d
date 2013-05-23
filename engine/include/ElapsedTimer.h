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
#ifndef ELAPSED_TIMER_H_
#define ELAPSED_TIMER_H_

#include <Object.h>

namespace yam2d
{

/**
 * Class for ElapsedTimer. 
 * 
 * Elapsed timer is handy tool for getting elapsed time in simulation. Typical
 * Elapsed timer usage is following: Create new instance of ElapsedTimer. Call
 * reset, when you want to start/restart timer. Call getTime, when you want to know,
 * how much time has been elapsed since last reset call.
 *
 * Elapsed timer uses Windows GetTime function for internal operation, so the resolution
 * of the timer is not best possible.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com)
 */
class ElapsedTimer : public Object
{
public:
	/**
	 * Default constructor.
	 *
	 * Creates new elapsed timer and initializes it. You must call reset before first getTime call.
	 */
	ElapsedTimer();

	virtual ~ElapsedTimer();

	/** Resets timer. */
	void reset();

	/** Returns elapsed time in seconds since last call to reset. */
	float getTime() const;

private:
#if defined(_WIN32)
	typedef __int64 YAM_TIME_TYPE;
#else
	typedef long YAM_TIME_TYPE;
#endif
	YAM_TIME_TYPE m_startTime;
};

}

#endif // ELAPSED_TIMER_H_




