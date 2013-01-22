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

//#define WIN32_LEAN_AND_MEAN
#include <windowsx.h>
#include "es_util.h"
#include "es_util_win.h"
#include <es_assert.h>
#include <config.h>
#include <ElapsedTimer.h>

namespace yam2d
{

// anonymous namespace for internal functions
namespace
{

int xPos = 0;//GET_X_LPARAM(lParam); 
int yPos = 0;//GET_Y_LPARAM(lParam); 
bool leftClicked = false;//(wParam & MK_LBUTTON) != 0;
bool rightClicked = false;//(wParam & MK_RBUTTON) != 0;
bool middleClicked = false;// (wParam & MK_MBUTTON) != 0;


LRESULT WINAPI ESWindowProc ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
	LRESULT  lRet = 1; 
	assert( hWnd != 0 );
	switch (uMsg) 
	{
	case WM_CREATE:
		break;

	case WM_PAINT:
		{
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr (hWnd, GWL_USERDATA );
			assert( esContext != 0 );

			if ( esContext->drawFunc )
			{
				esContext->drawFunc ( esContext );
				eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
			}   

			ValidateRect( esContext->hWnd, NULL );
		}
		break;
	
	case WM_SIZING:
		{
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			assert( esContext != 0 );
			RECT rc;
			GetClientRect(hWnd,&rc);
			esContext->height = rc.bottom - rc.top;
			esContext->width = rc.right - rc.left;
		}
		break;

	case WM_SIZE:
		{
			RECT rc;
			GetClientRect(hWnd,&rc);
			int w = rc.right - rc.left;
			int h = rc.bottom - rc.top;
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 )
			{
				esContext->height = h;
				esContext->width = w;
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);             
		break; 
      

	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		leftClicked = true;
		mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		break;

	case WM_LBUTTONUP:
		SetCapture(0);
		leftClicked = false;
		mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		break;

	case WM_RBUTTONDOWN:
		SetCapture(hWnd);
		rightClicked = true;
		mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		break;
	case WM_RBUTTONUP:
		SetCapture(0);
		rightClicked = false;
		mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		break;

	case WM_MBUTTONDOWN:
		SetCapture(hWnd);
		middleClicked = true;
		mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		break;
	case WM_MBUTTONUP:
		SetCapture(0);
		middleClicked = false;
		mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		break;

	case  WM_CAPTURECHANGED:
		middleClicked = false;
		rightClicked = false;
		leftClicked = false;
		mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		break;

	case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouseWheel( delta/WHEEL_DELTA );
		}
		break;

	case WM_MOUSEMOVE:
		{
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			assert( esContext != 0 );
			POINT point;
			point.x = GET_X_LPARAM(lParam);
			point.y = GET_Y_LPARAM(lParam);
			MapWindowPoints(HWND_DESKTOP,hWnd,&point,1);

			xPos = point.x;
			yPos = point.y;
		
			if( (wParam & MK_LBUTTON) == 0 )
			{
				leftClicked = false;
			}

			mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		}
		break;

	default: 
		lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}

}


GLboolean winCreate ( ESContext *esContext, const char *title, bool resizable )
{
	assert( esContext != 0 );

	WNDCLASS wndclass = {0}; 
	DWORD    wStyle   = 0;
	RECT     windowRect;
	HINSTANCE hInstance = GetModuleHandle(NULL);


	wndclass.style         = CS_OWNDC;
	wndclass.lpfnWndProc   = (WNDPROC)ESWindowProc; 
	wndclass.hInstance     = hInstance; 
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclass.lpszClassName = "opengles1.x"; 

	if (!RegisterClass (&wndclass) ) 
	{
		esLogEngineError("Failed to register wndclass");
		return FALSE; 
	}

	if( resizable )
	{
		wStyle = WS_VISIBLE | WS_MAXIMIZEBOX | WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME;
	}
	else
	{
		wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;
	}

	// Adjust the window rectangle so that the client area has
	// the correct number of pixels
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = esContext->width;
	windowRect.bottom = esContext->height;

	AdjustWindowRect ( &windowRect, wStyle, FALSE );
	
	esContext->hWnd = CreateWindow(
									"opengles1.x",
									title,
									wStyle,
									10,
									10,
									windowRect.right - windowRect.left,
									windowRect.bottom - windowRect.top,
									NULL,
									NULL,
									hInstance,
									NULL);

	if ( esContext->hWnd == NULL )
	{
		esLogEngineError("Failed to create window");
		return GL_FALSE;
	}

	// Set user data
	SetWindowLongPtr( esContext->hWnd, GWL_USERDATA, (LONG) (LONG_PTR) esContext );
	
	// Show window
	ShowWindow ( esContext->hWnd, TRUE );

	return GL_TRUE;
}


void winLoop ( ESContext *esContext )
{
	assert( esContext != 0 );
	MSG msg = { 0 };
	bool done = false;
	ElapsedTimer timer;
	timer.reset();

	while (!done)
	{
		int gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);

		if ( gotMsg )
		{
			if (msg.message==WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
		else
		{
			SendMessage( esContext->hWnd, WM_PAINT, 0, 0 );
		}

		// Call update function if registered
		if ( !done && esContext->updateFunc != NULL )
		{
			float deltaTime = timer.getTime();
			if( deltaTime > 0.0f )
			{
				esContext->updateFunc ( esContext, deltaTime );
			}
			timer.reset();
			clearInput();
		}
	}

	if ( esContext->deinitFunc != NULL )
	{
		esContext->deinitFunc ( esContext );
	}
}

}

