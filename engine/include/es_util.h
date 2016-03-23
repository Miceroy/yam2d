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
#ifndef ESUTIL_H_
#define ESUTIL_H_

#if defined(_WIN32)
#include <OGLES/Include/GLES/gl.h>
#include <OGLES/Include/EGL/egl.h>
#else
#include <EGL/egl.h>
#include <GLES/gl.h>
#endif


#include <vector>
#include <string>

#if defined(ANDROID)
struct android_app;
class ASensorManager;
class ASensor;
class ASensorEventQueue;
extern "C" void app_dummy();
#define YAM2D_START app_dummy();
#endif

#if defined(_WIN32)
#define YAM2D_START
#endif

namespace yam2d
{

/**
 * Flags for creating window isong esCreateWindow function. Flags can be combined
 * either using or operator '|' or '+' sign.
 */
enum WindowFlag
{
	/// RGB color buffer. Color buffer should have R,G,B channels
	ES_WINDOW_RGB =			0,
	/// ALPHA color buffer. Color buffer should have alpha.
	ES_WINDOW_ALPHA =		1, 
	/// depth buffer should be created.
	ES_WINDOW_DEPTH =		2,
	/// stencil buffer should be created
	ES_WINDOW_STENCIL =		4,
	/// multi-sample buffer should be created
	ES_WINDOW_MULTISAMPLE =	8,
	/// resizable window should be created
	ES_WINDOW_RESIZEABLE =	16,
	/// default flags
	ES_WINDOW_DEFAULT = ES_WINDOW_RGB|ES_WINDOW_ALPHA|ES_WINDOW_DEPTH
};



enum TouchEventType
{
	TOUCH_BEGIN = 0,
	TOUCH_MOVE,
	TOUCH_CANCEL,
	TOUCH_END
};

/**
 * Application context.
 */
struct ESContext
{
	/// Window width
	GLint       width;

	/// Window height
	GLint       height;
	
	/// EGL display
	EGLDisplay  eglDisplay;
      
	/// EGL context
	EGLContext  eglContext;

	/// EGL surface
	EGLSurface  eglSurface;

	bool quitFlag;

	/// Callbacks
	bool (*initFunc) ( ESContext* );
	void (*drawFunc) ( ESContext* );
	void (*updateFunc) ( ESContext*, float deltaTime );
	void (*deinitFunc) ( ESContext* );
	void (*touchEventFunc) ( ESContext*, TouchEventType type, int touchId, int x, int y );
#if defined(_WIN32)
	/// Window handle
	EGLNativeWindowType  hWnd;
#elif defined(ANDROID)
	struct android_app* app;
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
	GLint windowCreateFlags;
    ASensorEventQueue* sensorEventQueue;
#endif

};


/**
 * Initialize ES framework context.  This must be called before calling any other functions.
 * 
 * @param esContext Application context
 */

#if defined(ANDROID)
void esInitContext_android( ESContext *esContext );
#endif
static inline void esInitContext ( ESContext *esContext )
{
	YAM2D_START
//	assert ( esContext != NULL );
	memset( esContext, 0, sizeof( ESContext) );

#if defined(ANDROID)
	esInitContext_android( esContext );
#endif
}


/**
 * Create a window with the specified parameters.
 * 
 * @param esContext Application context
 * @param title Name for title bar of window
 * @param width Width in pixels of window to create
 * @param height Height in pixels of window to create
 * @param flags Bitfield for the window creation flags see WindowFlag. 
 *
 * @return GL_TRUE if window creation is succesful, GL_FALSE otherwise
 */
GLboolean esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLint flags );


/**
 * Start the main loop for the OpenGL ES application.
 *
 * @param esContext Application context
 */
void esMainLoop ( ESContext *esContext );

/**
 Quits application.
 */
void esQuitApp(ESContext *esContext);

/**
 * Register a init callback function to be used to init the game.
 *
 * @param esContext Application context
 * @param drawFunc Draw callback function that will be used to render the scene
 */
void esRegisterInitFunc ( ESContext *esContext, bool (*initFunc) ( ESContext* ) );


/**
 * Register a draw callback function to be used to render each frame.
 *
 * @param esContext Application context
 * @param drawFunc Draw callback function that will be used to render the scene
 */
void esRegisterDrawFunc ( ESContext *esContext, void (*drawFunc) ( ESContext* ) );


/**
 * Register an update callback function to be used to update on each time step.
 *
 * @param esContext Application context
 * @param updateFunc Update callback function that will be used to render the scene
 */
void esRegisterUpdateFunc ( ESContext *esContext, void (*updateFunc) ( ESContext*, float ) );


/**
 * Register a deinit callback function to be used to deallocate resources.
 *
 * @param esContext Application context
 * @param deinitFunc Deinit callback function
 */
void esRegisterDeinitFunc ( ESContext *esContext, void (*deinitFunc) ( ESContext* ) );


/**
 * Register an keyboard input processing callback function.
 *
 * @param esContext Application context
 * @param keyFunc Key callback function for application processing of keyboard input
 */
void esRegisterKeyFunc ( ESContext *esContext, void (*drawFunc) ( ESContext*, unsigned char, int, int ) );

void esRegisterTouchEventFunc ( ESContext *esContext, void (*touchEventFunc) ( ESContext*, TouchEventType type, int touchId, int x, int y ) );

ESContext* esGetCurrentContext();

/**
 * Log a message to the debug output for the platform.
 *
 * @param formatStr Format string for error log.  
 */
void esLogMessage ( const char *formatStr, ... );


/**
 * Loads a 24-bit PNG image from a file.
 *
 * @param fileName Name of the file on disk
 * @param buffer Bitmap data. Can be NULL, if data is NULL, then only width, height and bit depth is read.
 * @param width Width of loaded image in pixels
 * @param height Height of loaded image in pixels
 * @param bytesPerPixel Bit depth of the image 3=RGB, 4=RGBA
 *
 * @return True if success.
 */
bool esLoadPNG (const char *fileName, unsigned char *buffer, int *width, int *height, int *bytesPerPixel);


/**
 * Sets viewport with tear edges.
 */
void esViewportTearEdges(int sx, int sy, float desiredAspectRatio);

/**
 * Returns file path, from full file name
 * 
 * Function detects bots forward slash and back slash as directory separator. If path exist in 
 * given fileName, returned string contains trailing slash or back slash, depending on given 
 * input string format. If path does not existin given fileName, returned value is zero length string.
 */
std::string getPath( const std::string& fileName );


/**
 * Reads file data to vector.
 */
std::vector<unsigned short> readFile(const char* const fileName);


}


#endif


