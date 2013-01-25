// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Camera class
#include <Camera.h>

#include <Tileset.h>
#include <Layer.h>
#include <Tile.h>


using namespace yam2d;

namespace
{
	// Pointer to TmxMap-object
	TmxMap* map = 0;

	// Way 1 is to use so called callback functions for creating different kind of objects.
		
	Tileset* createNewTileset(void* userData, const std::string& name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties )
	{
		esLogMessage("Creating my own tileset!");
		return new Tileset(name, spriteSheet, tileOffsetX, tileOffsetY, properties);
	}
	
	Layer* createNewLayer(void* userData, Map* map, const std::string& name, float opacity, bool visible, const PropertySet& properties)
	{		
		esLogMessage("Creating my own layer!");

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

	Tile* createNewTile(void* userData, Map* map, Layer* layer, const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally, const PropertySet& properties)
	{
	//	esLogMessage("Creating my own tile!");
		return new Tile(0, position, tileset, id, flippedHorizontally, flippedVertically, flippedDiagonally, map->getTileWidth(), map->getTileHeight());
	}

	GameObject* createNewGameObject(void* userData, Map* map, Layer* layer, const std::string& type, const vec2& position, const vec2& size, const std::string& name, const PropertySet& properties)
	{
	//	esLogMessage("Creating my own game object!");
		return new GameObject(0, position, size, name );
	}
}


// Initialize the game
bool init ( ESContext *esContext )
{
	// Create new TmxMap object
	map = new TmxMap();

	// Register create callbacks. All callbacks is not needed to be registered. In that case, default behaviour of TmxMap is used.
	map->registerCreateNewTilesetFunc(createNewTileset);
	map->registerCreateNewLayerFunc(createNewLayer);
	map->registerCreateNewTileFunc(createNewTile);
	map->registerCreateNewGameObjectFunc(createNewGameObject);

	// Additionally you can register one void-pointer to be passed for each create function call in userData param.
	// map->setCallBackData( myPointer ) 

	// Load map file
	bool okay = map->loadMapFile("assets/level.tmx");

	if( okay )
	{
		// Move camera to middle of map.
		map->getCamera()->setPosition( vec2(map->getWidth()/2.0f - 0.5f, map->getHeight()/2.0f - 0.5f));
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
	esCreateWindow( &esContext, "Custom map creation", 1280, 720, ES_WINDOW_DEFAULT );

	if ( !init ( &esContext ) )
		return 0;

	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
	esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
