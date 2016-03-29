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
#include <windows.h>
#include "es_util.h"
#include "es_util_win32.h"
#include <es_assert.h>
#include <config.h>
#include <ElapsedTimer.h>

namespace yam2d
{

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

// anonymous namespace for internal functions
namespace
{

static int xPos = 0;//GET_X_LPARAM(lParam); 
static int yPos = 0;//GET_Y_LPARAM(lParam); 
static bool leftClicked = false;//(wParam & MK_LBUTTON) != 0;
static bool rightClicked = false;//(wParam & MK_RBUTTON) != 0;
static bool middleClicked = false;// (wParam & MK_MBUTTON) != 0;
static bool g_firstUpdateDone = false;
static bool done = false;
static ESContext * g_lastCtx = 0;



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

			if ( esContext->drawFunc && g_firstUpdateDone )
			{
				try
				{
					if ( esContext->drawFunc && esContext->width > 0 && esContext->height > 0)
					{
						esContext->drawFunc ( esContext );
						eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
					}   
				}
				catch (std::exception& e)
				{
					printf("std::exception: %s\n", e.what());
					done = true;
					DebugBreak();
				}
				catch (...)
				{
					printf("Unknown exception ocurred!\n");
					done = true;
					DebugBreak();
				}
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
		done = true;
		PostQuitMessage(0);             
		break; 
      

	case WM_LBUTTONDOWN:
		{
			SetCapture(hWnd);
			leftClicked = true;
			mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
		
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 )
			{
				touchEventFunc(esContext, TOUCH_BEGIN, 0, xPos, yPos ); 
			}
		}
		break;

	case WM_LBUTTONUP:
		{			
			mouseState(false, rightClicked, middleClicked, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 && leftClicked )
			{
				touchEventFunc(esContext, TOUCH_END, 0, xPos, yPos ); 
			}
			leftClicked = false;
			SetCapture(leftClicked||rightClicked||middleClicked ? hWnd : 0);
		}
		break;

	case WM_RBUTTONDOWN:
		{
			SetCapture(hWnd);
			rightClicked = true;
			mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 )
			{
				touchEventFunc(esContext, TOUCH_BEGIN, 1, xPos, yPos ); 
			}
		}
		break;

	case WM_RBUTTONUP:
		{			
			mouseState(leftClicked, false, middleClicked, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 && rightClicked )
			{
				touchEventFunc(esContext, TOUCH_END, 1, xPos, yPos ); 
			}
			rightClicked = false;
			SetCapture(leftClicked||rightClicked||middleClicked ? hWnd : 0);
		}
		break;

	case WM_MBUTTONDOWN:
		{
			SetCapture(hWnd);
			middleClicked = true;
			mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 )
			{
				touchEventFunc(esContext, TOUCH_BEGIN, 2, xPos, yPos ); 
			}
		}
		break;

	case WM_MBUTTONUP:
		{		
			mouseState(leftClicked, rightClicked, false, xPos, yPos);

			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( esContext != 0 && middleClicked)
			{
				touchEventFunc(esContext, TOUCH_END, 2, xPos, yPos ); 
			}
			middleClicked = false;
			SetCapture(leftClicked||rightClicked||middleClicked ? hWnd : 0);
		}
		break;

	case  WM_CAPTURECHANGED:
		{
			if( GetCapture() == 0 )
			{
				mouseState(false, false, false, xPos, yPos);
		
				ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
				if( esContext != 0 )
				{
					if( leftClicked ) touchEventFunc(esContext, TOUCH_CANCEL, 0, xPos, yPos ); 
					if( rightClicked ) touchEventFunc(esContext, TOUCH_CANCEL, 1, xPos, yPos ); 
					if( middleClicked ) touchEventFunc(esContext, TOUCH_CANCEL, 2, xPos, yPos ); 
				}
			
				middleClicked = false;
				rightClicked = false;
				leftClicked = false;
			}
		}		
		break;

	case WM_MOUSEWHEEL:
		{ 
			//esLogEngineError("WM_MOUSEWHEEL");
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			mouseWheel( delta/WHEEL_DELTA );
		}
		break;

	case WM_MOUSEMOVE:
		{
			POINT point;
			point.x = GET_X_LPARAM(lParam);
			point.y = GET_Y_LPARAM(lParam);
			ClientToScreen(hWnd,&point);
			MapWindowPoints(HWND_DESKTOP,hWnd,&point,1);
			ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );
			if( point.x < 0 || point.y < 0 ||
				point.x >= esContext->width ||  point.y >= esContext->height )
			{
				SetCapture(0);
			}
			else
			{
				xPos = point.x;
				yPos = point.y;
			
				if( esContext != 0 )
				{
					if( leftClicked ) touchEventFunc(esContext, TOUCH_MOVE, 0, xPos, yPos ); 
					if( rightClicked ) touchEventFunc(esContext, TOUCH_MOVE, 1, xPos, yPos ); 
					if( middleClicked ) touchEventFunc(esContext, TOUCH_MOVE, 2, xPos, yPos ); 
				}

				mouseState(leftClicked, rightClicked, middleClicked, xPos, yPos);
			}

			
		}
		break;

	default: 
		lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}

}

ESContext *esGetCurrentContext()
{
	return g_lastCtx;
}

GLboolean winCreate ( ESContext *esContext, const char *title, bool resizable )
{
	assert( esContext != 0 );
	g_lastCtx = esContext;
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
//		return FALSE; 
	}

	if( resizable )
	{
		wStyle = WS_VISIBLE | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_BORDER | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME;
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
	
	esContext->hWnd = CreateWindowEx(WS_EX_LEFT,
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
		DWORD LastError = GetLastError();
		(void) LastError;
		esLogEngineError("Failed to create window");
//		return GL_FALSE;
	}

	// Set user data
	SetWindowLongPtr( esContext->hWnd, GWL_USERDATA, (LONG) (LONG_PTR) esContext );
	
	// Show window
	ShowWindow ( esContext->hWnd, SW_SHOWDEFAULT );

	return GL_TRUE;
}


void winLoop ( ESContext *esContext )
{
	// We must set thread affinity mask for this thread, inorder that QueryPerformanceCounter does not cause troubles for us.
	// See atricle "Game Timing and Multicore Processors (Windows)" http://msdn.microsoft.com/en-us/library/windows/desktop/ee417693(v=vs.85).aspx
	// Enable only core 1, by using mask 0x01.
	SetThreadAffinityMask(GetCurrentThread(), 0x01);
	
	done = false;
	assert( esContext != 0 );
	MSG msg = { 0 };
	ElapsedTimer timer;
	timer.reset();

	//bool gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0);
	if( false == esContext->initFunc(esContext) )
	{
		return;
	}

	while (!done)
	{
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if( msg.message==WM_QUIT )
			{
				done = true;
				continue;
			}

			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}
		else
		{		
			// Call update function if registered
			if ( esContext->updateFunc != NULL )
			{
				float deltaTime = timer.getTime();
				timer.reset();
				if( deltaTime > 0.0f )
				{
					try
					{
						esContext->updateFunc ( esContext, deltaTime );
						g_firstUpdateDone = true;
						clearInput();
					}
					catch (std::exception& e)
					{
						printf("std::exception: %s\n", e.what());
						done = true;
						DebugBreak();
					}
					catch (...)
					{
						printf("Unknown exception ocurred!\n");
						done = true;
						DebugBreak();
					}

					if (esContext->quitFlag)
						done = true;
				}			
			}

			if( !done )
			{
				SendMessage( esContext->hWnd, WM_PAINT, 0, 0 );
			}
		}
	}

	if ( esContext->deinitFunc != NULL )
	{
		esContext->deinitFunc ( esContext );
	}
}

EGLBoolean CreateEGL11Context ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[])
{
	assert(eglDisplay != 0);
	assert(eglSurface != 0);
	assert(eglContext != 0);

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	// Get Display
	display = eglGetDisplay(GetDC(hWnd));
	if ( display == EGL_NO_DISPLAY )
	{
		esLogEngineError("eglGetDisplay failed");
//		return EGL_FALSE;
	}

	// Initialize EGL
	if ( !eglInitialize(display, &majorVersion, &minorVersion) )
	{
		esLogEngineError("eglInitialize failed");
//		return EGL_FALSE;
	}

	// Get configs
	if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
	{
		esLogEngineError("eglGetConfigs failed");
//		return EGL_FALSE;
	}

	// Choose config
	if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
	{
		esLogEngineError("eglChooseConfig failed");
//		return EGL_FALSE;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
	if ( surface == EGL_NO_SURFACE )
	{
		esLogEngineError("eglCreateWindowSurface failed");
//		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
	if ( context == EGL_NO_CONTEXT )
	{
		esLogEngineError("eglCreateContext failed");
//		return EGL_FALSE;
	}   
   
	// Make the context current
	if ( !eglMakeCurrent(display, surface, surface, context) )
	{
		esLogEngineError("eglMakeCurrent failed");
//		return EGL_FALSE;
	}
   
	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;
	return EGL_TRUE;
} 

/*

void esInitContext ( ESContext *esContext )
{
	YAM2D_START
	assert ( esContext != NULL );
	memset( esContext, 0, sizeof( ESContext) );

}
*/

GLboolean esCreateWindow ( ESContext *esContext, const char* title, GLint width, GLint height, GLint flags )
{
	assert( esContext != 0 );
	EGLint attribList[] =
	{
		EGL_RED_SIZE,       8,
		EGL_GREEN_SIZE,     8,
		EGL_BLUE_SIZE,      8,
		EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};
   
	if ( esContext == NULL )
	{
		esLogEngineError("Given ESContext is NULL");
//		return GL_FALSE;
	}

	esContext->width = width;
	esContext->height = height;

	if ( !winCreate ( esContext, title, (flags&ES_WINDOW_RESIZEABLE) ? true : false ) )
	{
		return GL_FALSE;
	}

  
	if ( !CreateEGL11Context( esContext->hWnd, &esContext->eglDisplay, &esContext->eglContext,
							&esContext->eglSurface,	attribList) )
	{
		return GL_FALSE;
	}
   

	return GL_TRUE;
}


void esMainLoop ( ESContext *esContext )
{
	winLoop( esContext );
}


}

