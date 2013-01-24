// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Camera class
#include <Camera.h>


using namespace yam2d;

namespace
{
	// Pointer to TmxMap-object
	TmxMap* map = 0;
}



// Initialize the game
bool init ( ESContext *esContext )
{
	// Create new TmxMap object
	map = new TmxMap();

	// Load map file
	bool okay = map->loadMapFile("level.tmx");

	if( okay )
	{
		// Move camera to middle of map.
		map->getCamera()->setPosition( vec2(map->getWidth()/2.0f-0.5f, map->getHeight()/2.0f-0.5f));
	}

	return okay;
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
	map->getCamera()->setScreenSize(esContext->width,esContext->height); 

	// Render map and all of its layers containing GameObjects to screen.
	map->render();
}


int main ( int argc, char *argv[] )
{
	ESContext esContext;
	esInitContext ( &esContext );
	esCreateWindow( &esContext, "TMX map loading", 1280, 720, ES_WINDOW_DEFAULT );

	if ( !init ( &esContext ) )
		return 0;

	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
	esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
