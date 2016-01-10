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
#include "es_util_win32.h"
#include <stdio.h>
#include <stdlib.h>
#include <OGLES/Include/GLES/gl.h>
#include <OGLES/Include/EGL/egl.h>
#include <config.h>

#if defined(ANDROID)
#include <android/log.h>
#define printLog(...) ((void)__android_log_print(ANDROID_LOG_INFO, "yam2d", __VA_ARGS__))
#else
#define printLog printf
#endif

namespace yam2d
{

// anonymous namespace for internal functions
namespace
{
#if defined(ANDROID)
template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) {  return  __a < __b ? __b : __a; }
#endif

#if defined(YAM_WRITING_LOGS_TO_FILE)
	const char* const logFileName = 	YAM_WRITING_LOGS_TO_FILE;
	FILE* logFileHandle = (strlen(logFileName) > 0) ? fopen (logFileName,"w") : NULL;
#endif

	int toInteger(double v)
	{
		return int(v+0.5);
	}
}


void esRegisterInitFunc ( ESContext *esContext, bool (*initFunc) ( ESContext* ) )
{
	esContext->initFunc = initFunc;
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

void esRegisterTouchEventFunc ( ESContext *esContext, void (*touchEventFunc) ( ESContext*, TouchEventType type, int touchId, int x, int y ) )
{
	esContext->touchEventFunc = touchEventFunc;
}

void esLogMessage ( const char *formatStr, ... )
{
	va_list params;
	char buf[10*1024];

	va_start ( params, formatStr );
#if defined(_WIN32)
	vsprintf_s ( buf, sizeof(buf),  formatStr, params );
#else
	vsprintf( buf,  formatStr, params );
#endif

	printLog( "%s\n", buf );
    
#if defined(YAM_WRITING_LOGS_TO_FILE)
	if( logFileHandle != NULL )
	{
		fprintf(logFileHandle, "%s\n", buf );
	}
#endif

	va_end ( params );
}


void esLogEngineError( const char *formatStr, ... )
{
#if defined(ERROR_LOGS_ENABLED)
	va_list params;
	char buf[BUFSIZ];

	va_start ( params, formatStr );
#if defined(_WIN32)
	vsprintf_s ( buf, sizeof(buf),  formatStr, params );
#else
	vsprintf( buf,  formatStr, params );
#endif
    
	printLog( "Error: %s\n", buf );

#if defined(YAM_WRITING_LOGS_TO_FILE)
	if( logFileHandle != NULL )
	{
		fprintf(logFileHandle, "Error: %s\n", buf );
	}
#endif

	va_end ( params );

	std::string s = buf;
	throw std::string(s);
#else
	(void)formatStr;
#endif
}

void esLogEngineDebug( const char *formatStr, ... )
{
#if defined(DEBUG_LOGS_ENABLED)
	va_list params;
	char buf[BUFSIZ];

	va_start ( params, formatStr );
#if defined(_WIN32)
	vsprintf_s ( buf, sizeof(buf),  formatStr, params );
#else
	vsprintf( buf,  formatStr, params );
#endif
    
	printLog( "Engine: %s\n", buf );
    
	va_end ( params );
#else
	(void)formatStr;
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

void yamAssert(const char* expression, const char* file, int line )
{
	esLogEngineError( "Assertation failed at %s:%d: %s", file, line, expression );
}

std::string getPath( const std::string& fileName )
{
	std::string path = fileName;
	int last1 = path.find_last_of('\\');
	int last2 = path.find_last_of('/');
	
	int last = max(last1,last2);

	if( last > 0 )
	{		
		path = path.substr(0,last+1);
	}
	else
	{
		path = "";
	}

	return path;
}

std::string getFileName( const std::string& fileName )
{
	std::string path = fileName;
	int last1 = path.find_last_of('\\');
	int last2 = path.find_last_of('/');
	
	int last = max(last1,last2);

	if( last > 0 )
	{		
		path = path.substr(last+1, path.size());
	}
	else
	{
		path = fileName;
	}

	return path;
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
	(void)bytesRead;
	assert( bytesRead > 0 && bytesRead <= int(res.size()));
	fclose(fp);
	return res;
}

}



