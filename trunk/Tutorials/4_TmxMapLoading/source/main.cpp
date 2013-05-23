// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Camera class
#include <Camera.h>
#include <input.h>

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
	bool okay = map->loadMapFile("assets/level.tmx");

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


/** Moves gameobject if user presses arrow keys. Game object is moved according to speed. */
void moveGameObjectAccordingToKeypresses(GameObject* go, float speed, float deltaTime, bool moveDirectionIsRelativeToGameObjectRotation)
{
	// Get move direction from keyboard
	vec2 direction;

	if( moveDirectionIsRelativeToGameObjectRotation )
	{
		float forward = float(getKeyState(KEY_UP)-getKeyState(KEY_DOWN));
		float right = float(getKeyState(KEY_RIGHT)-getKeyState(KEY_LEFT));
		direction = rotateVector( vec2(forward,right), go->getRotation() );
	}
	else
	{
		direction.x = float(getKeyState(KEY_RIGHT)-getKeyState(KEY_LEFT));
		direction.y = float(getKeyState(KEY_DOWN)-getKeyState(KEY_UP));
	}

	if( direction.Length() < 0.001f )
		return; // no need to move

	// Make lenght of direction to 1
	direction.Normalize();

	
	// Velocity is direction times speed
	vec2 velocity = speed*direction;

	// Move delta is velocity times delta time.
	vec2 moveDelta = deltaTime*velocity;

	// New position is old position + move delta
	go->setPosition(go->getPosition() + moveDelta);
}

// Update game
void update( ESContext* ctx, float deltaTime )
{
	moveGameObjectAccordingToKeypresses(map->getCamera(), 5.0, deltaTime, false );

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
