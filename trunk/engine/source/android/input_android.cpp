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

#include "Input.h"


namespace yam2d
{



// anonymous namespace for internal functions
namespace
{
//	bool clicks[3];
//	int mouseXValue = 0;
//	int mouseYValue = 0;
//	int mouseWheelDelta = 0;

	std::vector< Touch > touches;
}

void clearInput()
{
//	mouseWheelDelta = 0;
}

/*
void mouseWheel(int mouseWheel)
{
//	mouseWheelDelta += mouseWheel;
}*/

/*
void mouseState(bool leftClicked, bool rightClicked, bool middleClicked, int mouseX, int mouseY )
{
	clicks[0] = leftClicked;
	clicks[1] = rightClicked;
	clicks[2] = middleClicked;
	mouseXValue = mouseX;
	mouseYValue = mouseY;
}*/

void touchEventFunc( ESContext* esContext, TouchEventType type, int touchId, int x, int y )
{
	if( touches.size() <= size_t(touchId) )
	{
		touches.resize(touchId+1);
	}

	touches[touchId].touchId = touchId;
	touches[touchId].x = x;
	touches[touchId].y = y;
	
	if( type == TOUCH_BEGIN || type == TOUCH_MOVE )
	{
		touches[touchId].pressed = true;
	}
	else
	{
		touches[touchId].pressed = false;
	}

	if( esContext != 0 && esContext->touchEventFunc != 0 )
	{
		esContext->touchEventFunc(esContext,type,touchId,x,y);
	}
}

/*int getMouseButtonState(MouseButtons button)
{
	return clicks[button];
}


int getMouseAxisX()
{
	return mouseXValue;
}

int getMouseAxisY()
{
	return mouseYValue;
}
*/
int getMouseWheelDelta()
{
	return 0;
}

int getKeyState(KeyCodes keyCode)
{
	return 0;
//	return (GetAsyncKeyState(keyCode) & 0x8000) ? 1 : 0;
}


const std::vector<Touch>& getActiveTouches()
{
	return touches;
}


}


