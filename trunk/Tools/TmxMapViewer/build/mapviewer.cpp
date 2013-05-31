// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Tile
#include <Tile.h>
// For keyboard
#include <Input.h>
#include <Camera.h>
#include <Layer.h>
#include <Texture.h>
#include <Windows.h> // SetCurrentDirectoryA

using namespace yam2d;

namespace
{
	// Pointer to TmxMap-object
	TmxMap* map = 0;

	// Zoom for camera
	float zoom = 1.0f;

	/** Moves map camera according to arrow keys. */
	void moveMapCameraFromKeyboard(Map* m, float deltaTime)
	{
		// Calculate camera move direction from key presses. 
		int dirX = getKeyState(KEY_RIGHT)-getKeyState(KEY_LEFT);
		int dirY = getKeyState(KEY_DOWN)-getKeyState(KEY_UP);

		float speed = 30.0f; // Tiles per second

		// Take old position.
		vec2 oldCameraPos = m->getCamera()->getPosition();

		// Calculate new postion: 
		// new pos = old pos + speed * direction * dt (So called euler integration)
		vec2 newCameraPos(0);
		newCameraPos.x = oldCameraPos.x + speed * dirX * deltaTime;
		newCameraPos.y = oldCameraPos.y + speed * dirY * deltaTime;

		// Set new postion
		m->getCamera()->setPosition(newCameraPos);
	}

	/** Moves map camera according to mouse drag. */
	void moveMapCameraFromMouseTracking(Map* m)
	{
		static bool trackingMouse = false;
		static float trackStartX = 0.0f;
		static float trackStartY = 0.0f;
		static vec2 originalCameraPos(0.0f);

		// Detect start of mouse down
		if(trackingMouse==false && getMouseButtonState(MOUSE_LEFT) )
		{
			// Start tracking
			trackingMouse = true;
			trackStartX = float(getMouseAxisX());
			trackStartY = float(getMouseAxisY());
			originalCameraPos = map->tileToScreenCoordinates( map->getCamera()->getPosition() );
		}

		// If mouse button released, stop tracking
		if( !getMouseButtonState(MOUSE_LEFT) )
		{
			trackingMouse = false;
		}

		// If tracking mouse
		if( trackingMouse == true )
		{
			// Callulate delta between start and end pos 
			float curX = float(getMouseAxisX());
			float curY = float(getMouseAxisY());
			vec2 delta((curX-trackStartX), (curY-trackStartY));

			// Convert screen coordinates to tile coordinates
			vec2 newCameraPos = map->screenToTileCoordinates(originalCameraPos.x-delta.x, originalCameraPos.y-delta.y);

			// Set new position to camera
			map->getCamera()->setPosition(newCameraPos);
		}
	}

}



// Initialize the game
bool init ( ESContext *esContext, char* mapFileName )
{
	// Create new TmxMap object
	map = new TmxMap();
	return  map->loadMapFile(mapFileName);
}

// Deinitialize the game
void deinit ( ESContext *esContext )
{
	// Delete map.
	delete map;
}


// Update game
void update( ESContext* ctx, float deltaTime )
{
	// Move camera according to keyboard	
	moveMapCameraFromKeyboard(map,deltaTime);
	
	// Move camera according to mouse
	moveMapCameraFromMouseTracking(map);

	// Zoom map from mouse wheel
	if( getMouseWheelDelta() != 0 )
	{
		if( getMouseWheelDelta() > 0 )
		{
			zoom *= 1.0f + getMouseWheelDelta();
		}

		if( getMouseWheelDelta() < 0 )
		{
			zoom /= 1.0f - getMouseWheelDelta();
		}
	}

	// Update map. this will update all GameObjects inside a map layers.
	map->update(deltaTime);
}


// Draw game
void draw ( ESContext *esContext )
{
	// Set OpenGL clear color (dark gray)
	glClearColor( 0.1f, 0.1f, 0.1f, 0.0f );

	// Clear the color buffer
	glClear ( GL_COLOR_BUFFER_BIT );

	// Set screen size to camera.
	map->getCamera()->setScreenSize( esContext->width, esContext->height, float(esContext->height)/float(zoom)); 

	// Render map and all of its layers containing GameObjects to screen.
	map->render();
}


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	if( strlen(lpCmdLine) == 0 )
	{
		MessageBox(0, L"Missing map file!", L"TMX map viewer", MB_ICONERROR );
		return 0;
	}	

	ESContext esContext;
	esInitContext ( &esContext );
	esCreateWindow( &esContext, "TMX map viewer", 1280, 720, ES_WINDOW_DEFAULT|ES_WINDOW_RESIZEABLE );

	// Instead of regular initialization, give second cmd 
	// argument to init function, which shall contain the 
	// map file name to be shown. (cmd argument is the 
	// full path of the executable in windows) 
	if ( !init ( &esContext, lpCmdLine ) )
	{
		MessageBox(0, L"Error loading map!", L"TMX map viewer", MB_ICONERROR );
		return 0;
	}

	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
	esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
