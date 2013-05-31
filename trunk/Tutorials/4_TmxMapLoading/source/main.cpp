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

	float zoom = 1.0f;

	float getZoom()
	{
		return zoom;
	}

	void setZoom(float newZoom)
	{
		zoom = b2Clamp(newZoom, 0.25f, 8.0f);
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

	// Variables for moveGameObjectAccordingToTouchTrack
	bool trackingMovement = false;
	int pressedIds[2];
	yam2d::vec2 pressedPos[2];


	/** Moves gameobject if user presses touch screen of left mouse button. Game object is moved according to move delta between. */
	void moveGameObjectAccordingToTouchTrack(GameObject* go, Map* m)
	{
		const std::vector<yam2d::Touch>& touches = getActiveTouches();
		
		int pressedCount = 0;
			
		for( size_t i=0; i<touches.size(); ++i )
		{
			if( touches[i].pressed )
			{
				pressedIds[pressedCount] = i;
				++pressedCount;
			}
			// Allow only 2 first
			if( pressedCount >= 2 )
				break;
		}

		if( trackingMovement == false && pressedCount == 1)
		{ 
			// Start tracking movement
			pressedPos[0].x = float(touches[pressedIds[0]].x);
			pressedPos[0].y = float(touches[pressedIds[0]].y);
		}

		trackingMovement = (pressedCount == 1);

		if( trackingMovement == true )
		{
			yam2d::vec2 currentPos;
			currentPos.x = float(touches[pressedIds[0]].x); 
			currentPos.y = float(touches[pressedIds[0]].y);
			vec2 delta = currentPos - pressedPos[0];

			if( delta.Length() > 0.1 )
			{
				delta = m->screenToTileCoordinates(delta);
				go->setPosition(go->getPosition() - delta );
				pressedPos[0] = currentPos;
			}

			pressedPos[0] = currentPos;
		}
	}
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
	esLogMessage("Deinit");
	delete map;
}


// Update game
void update( ESContext* ctx, float deltaTime )
{
	// Set zoom from mouse wheel
	setZoom( getZoom() - getMouseWheelDelta() );

	// Move camera in case of key presses
	moveGameObjectAccordingToKeypresses(map->getCamera(), 5.0f/getZoom(), deltaTime, false );
	moveGameObjectAccordingToTouchTrack(map->getCamera(), map);
	// Update map. this will update all GameObjects inside a map layers.
	map->update(deltaTime);
}


// Draw game
void draw ( ESContext *esContext )
{
	// Set OpenGL clear color (dark gray)
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

	// Clear the color & depth buffer
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Set screen size to camera.
	map->getCamera()->setScreenSize(esContext->width,esContext->height, 400/getZoom() ); 

	// Render map and all of its layers containing GameObjects to screen.
	map->render();
}


int main ( int argc, char *argv[] )
{
	ESContext esContext;
	esInitContext ( &esContext );
	esCreateWindow( &esContext, "TMX map loading", 1280, 720, ES_WINDOW_DEFAULT );

	esRegisterInitFunc( &esContext, init );
	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
    esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
