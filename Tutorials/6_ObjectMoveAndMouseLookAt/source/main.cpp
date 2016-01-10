// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Camera class
#include <Camera.h>
#include <Layer.h>
#include <Texture.h>
#include <SpriteComponent.h>
#include <Input.h>

using namespace yam2d;

namespace
{
	// Pointer to TmxMap-object
	TmxMap* map = 0;
	ComponentFactory* componentFactory = 0;

	// Pointer to game object
	GameObject* gameObject = 0;

	vec2 rotateVec2(const vec2& vec, float angle )
	{
		vec2 res;
		// Rotate vector according to angle (see http://en.wikipedia.org/wiki/Rotation_(mathematics))
		float sinAngle = sinf(angle);
		float cosAngle = cosf(angle);
		res.x = vec.x*cosAngle - vec.y*sinAngle;
		res.y = vec.x*sinAngle + vec.y*cosAngle;
		return res;
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
			direction = rotateVec2(vec2(forward, right), go->getRotation());
		}
		else
		{
			direction.x = float(getKeyState(KEY_RIGHT)-getKeyState(KEY_LEFT));
			direction.y = float(getKeyState(KEY_DOWN)-getKeyState(KEY_UP));
		}

		if( slm::length(direction) < 0.001f )
			return; // no need to move

		// Make lenght of direction to 1
		direction = slm::normalize(direction);

	
		// Velocity is direction times speed
		vec2 velocity = speed*direction;

		// Move delta is velocity times delta time.
		vec2 moveDelta = deltaTime*velocity;

		// New position is old position + move delta
		go->setPosition(go->getPosition() + moveDelta);
	}
}



// Initialize the game
bool init ( ESContext *esContext )
{
	// Create new TmxMap object
	map = new TmxMap();
	componentFactory = new DefaultComponentFactory();
	
	// Load map file
	if( !map->loadMapFile("level.tmx",componentFactory) )
		return false;

	// Create game object from "triangle.png, which transparent
	// color is pink and size is one tile (tile size comes from map)
	Texture* texture = new Texture("triangle.png");
	texture->setTransparentColor(255,0,255);

	gameObject = new GameObject(0, 0);
	SpriteComponent* sprite = new SpriteComponent(gameObject, texture);
	gameObject->setSize(map->getTileWidth(), map->getTileHeight());

	// add game object to map layer "GameObjects"
	map->getLayer("GameObjects")->addGameObject(gameObject);
	
	// Move camera to middle of map.
	map->getCamera()->setPosition( vec2(map->getWidth()/2.0f - 0.5f, map->getHeight()/2.0f - 0.5f));
	
	// Move gameobject to middle of map.
	gameObject->setPosition( vec2(map->getWidth()/2.0f - 0.5f, map->getHeight()/2.0f - 0.5f));

	gameObject->addComponent(sprite);

	return true;
}

// Deinitialize the game
void deinit ( ESContext *esContext )
{
	// Delete map.
	delete map;
	delete componentFactory;
}


// Update game
void update( ESContext* esContext, float deltaTime )
{
	
	map->getCamera()->setScreenSize(esContext->width,esContext->height, 720, 1280.0f/720.0f); 

	// Read mouse values
	float mouseX = float(getMouseAxisX());
	float mouseY = float(getMouseAxisY());

	// Convert mouse coordinates to map coordinates.
	vec2 mouseInMapCoordinates = map->screenToMapCoordinates(mouseX,mouseY);
	// Get vector from game object to mouse position.
	vec2 mouseRelativeToGameObject = mouseInMapCoordinates-gameObject->getPosition();
	// Get angle from x and y coordinates using atan2 function.
	esLogMessage("M: <%2.2f,%2.2f>", mouseRelativeToGameObject.x, mouseRelativeToGameObject.y);
	float rotation = atan2(mouseRelativeToGameObject.y, mouseRelativeToGameObject.x);
	// set new rotation to game object.
	gameObject->setRotation(rotation);

	// Move game object according to arrow keys.
	moveGameObjectAccordingToKeypresses(gameObject,2.0f,deltaTime,true);
	//moveGameObjectAccordingToKeypresses(gameObject,2.0f,deltaTime,false);
	
	// Update map. this will update all GameObjects inside a map layers.
	map->update(deltaTime);
}


// Draw game
void draw ( ESContext *esContext )
{
	// Set OpenGL clear color (dark gray)
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

	// Clear the color buffer
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Set screen size to camera.
	map->getCamera()->setScreenSize(esContext->width,esContext->height, 720, 1280.0f/720.0f); 

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
