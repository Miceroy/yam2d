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

#include "es_util.h"
#include <es_assert.h>
#include "es_util_win.h"
#include <stdio.h>
#include <stdlib.h>
#include <OGLES/Include/GLES/gl.h>
#include <OGLES/Include/EGL/egl.h>
#include <config.h>

namespace yam2d
{

// anonymous namespace for internal functions
namespace
{
	int toInteger(double v)
	{
		return int(v+0.5);
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
		return EGL_FALSE;
	}

	// Initialize EGL
	if ( !eglInitialize(display, &majorVersion, &minorVersion) )
	{
		esLogEngineError("eglInitialize failed");
		return EGL_FALSE;
	}

	// Get configs
	if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
	{
		esLogEngineError("eglGetConfigs failed");
		return EGL_FALSE;
	}

	// Choose config
	if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
	{
		esLogEngineError("eglChooseConfig failed");
		return EGL_FALSE;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
	if ( surface == EGL_NO_SURFACE )
	{
		esLogEngineError("eglCreateWindowSurface failed");
		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
	if ( context == EGL_NO_CONTEXT )
	{
		esLogEngineError("eglCreateContext failed");
		return EGL_FALSE;
	}   
   
	// Make the context current
	if ( !eglMakeCurrent(display, surface, surface, context) )
	{
		esLogEngineError("eglMakeCurrent failed");
		return EGL_FALSE;
	}
   
	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;
	return EGL_TRUE;
} 


void esInitContext ( ESContext *esContext )
{
	assert ( esContext != NULL );
	memset( esContext, 0, sizeof( ESContext) );
}


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
		return GL_FALSE;
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


void esRegisterDrawFunc ( ESContext *esContext, void (*drawFunc) (ESContext* ) )
{
	esContext->drawFunc = drawFunc;
}


void esRegisterUpdateFunc ( ESContext *esContext, void (*updateFunc) ( ESContext*, float ) )
{
	esContext->updateFunc = updateFunc;
}


void esRegisterDeinitFunc ( ESContext *esContext, void (*deinitFunc) ( ESContext* ) )
{
	esContext->deinitFunc = deinitFunc;
}


void esLogMessage ( const char *formatStr, ... )
{
	va_list params;
	char buf[BUFSIZ];

	va_start ( params, formatStr );
	vsprintf_s ( buf, sizeof(buf),  formatStr, params );
    
	printf ( "%s\n", buf );
    
	va_end ( params );
}


void esLogEngineError( const char *formatStr, ... )
{
#if defined(ERROR_LOGS_ENABLED)
	va_list params;
	char buf[BUFSIZ];

	va_start ( params, formatStr );
	vsprintf_s ( buf, sizeof(buf),  formatStr, params );
    
	printf ( "Error: %s\n", buf );
    
	va_end ( params );
#endif
}

void esLogEngineDebug( const char *formatStr, ... )
{
#if defined(DEBUG_LOGS_ENABLED)
	va_list params;
	char buf[BUFSIZ];

	va_start ( params, formatStr );
	vsprintf_s ( buf, sizeof(buf),  formatStr, params );
    
	printf ( "Engine: %s\n", buf );
    
	va_end ( params );
#endif
}


void esViewportTearEdges(int sx, int sy, float desiredAspectRatio)
{
	double w = double(sx);
	double h = double(sy);
	double curAspect = w/h;
		
	if( curAspect < desiredAspectRatio )
	{
		double desiredHeight = w/desiredAspectRatio;
		double d = 1.0 - desiredHeight/h;
		double margin = 0.5 * d;
		glViewport(  0, toInteger(margin*h), toInteger(1.0*w), toInteger((1.0 - (2.0*margin))*h) );
	}
	else if( curAspect > desiredAspectRatio )
	{
		double desiredWidth = h*desiredAspectRatio;
		double d = 1.0 - desiredWidth/w;
		double margin = 0.5 * d;
		glViewport( toInteger(margin*w), 0, toInteger((1.0 - (2.0*margin))*w), toInteger(h) );
	}
	else
	{
		glViewport( 0, 0, sx, sy );
	}
}


std::vector<unsigned short> readFile(const char* const fileName)
{
	FILE* fp = fopen (fileName,"rb");
	std::vector<unsigned short> res;
	if (fp == NULL)
	{
		return res;
	}

	int start = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int end = ftell(fp);
	res.resize((end-start)/2);
	fseek(fp,start,SEEK_SET);
	int bytesRead = fread(&res[0],2, res.size(), fp);

	fclose(fp);
	return res;
}

}



