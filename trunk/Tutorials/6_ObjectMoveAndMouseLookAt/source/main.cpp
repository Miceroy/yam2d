// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Camera class
#include <Camera.h>
#include <Layer.h>
#include <Texture.h>
#include <SpriteGameObject.h>
#include <Input.h>

using namespace yam2d;

namespace
{
	// Pointer to TmxMap-object
	TmxMap* map = 0;

	// Pointer to game object
	SpriteGameObject* gameObject = 0;

	// Custom layer function for supporting static layers.
	Layer* createNewLayer(void* userData, Map* map, const std::string& name, float opacity, bool visible, const PropertySet& properties)
	{		
		// Check that if "static"-propery is set to Leyer properties, and if it's value is "true" or 1
		if( properties.hasProperty("static") && 
			(properties.getLiteralProperty("static")=="true" || properties.getLiteralProperty("static")=="1")  )
		{
			// Is, so, then create static layer, which is batched only once, at first call to Map::render
			return new Layer(map, name, opacity, visible, true, properties);
		}

		// by default create dynamic layer, which is batced each frame
		return new Layer(map, name, opacity, visible, false, properties); 
	}

	vec2 rotate(const vec2& vec, float angle )
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
			direction = rotate( vec2(forward,right), gameObject->getRotation() );
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
		gameObject->setPosition(gameObject->getPosition() + moveDelta);
	}
}



// Initialize the game
bool init ( ESContext *esContext )
{
	// Create new TmxMap object
	map = new TmxMap();
	map->registerCreateNewLayerFunc(createNewLayer);

	// Load map file
	if( !map->loadMapFile("level.tmx") )
		return false;

	// Create game object from "triangle.png, which transparent
	// color is pink and size is one tile (tile size comes from map)
	Texture* texture = new Texture("triangle.png");
	texture->setTransparentColor(255,0,255);
	gameObject = new SpriteGameObject(0,texture);
	gameObject->setSize(map->getTileWidth(), map->getTileHeight());

	// add game object to map layer "GameObjects"
	map->getLayer("GameObjects")->addGameObject(gameObject);
	
	// Move camera to middle of map.
	map->getCamera()->setPosition( vec2(map->getWidth()/2.0f - 0.5f, map->getHeight()/2.0f - 0.5f));
	
	// Move gameobject to middle of map.
	gameObject->setPosition( vec2(map->getWidth()/2.0f - 0.5f, map->getHeight()/2.0f - 0.5f));

	return true;
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
	glClearColor( 0.1f, 0.1f, 0.1f, 0.0f );

	// Clear the color buffer
	glClear ( GL_COLOR_BUFFER_BIT );

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
