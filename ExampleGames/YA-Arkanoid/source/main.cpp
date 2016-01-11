// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Tile
#include <TileComponent.h>
#include <SpriteComponent.h>
// Layer
#include <Layer.h>
// Texture
#include <Camera.h>

using namespace yam2d;

namespace
{
	Map* map = 0;
}



// Initialize the game
bool init(ESContext *esContext)
{
	// Level tile size
	vec2 tileSize(1, 1);

	// Create new map, which tile width == tile height == 1 pixels/tile
	map = new Map(tileSize.x, tileSize.y);

	// Create "Background" and "Objects" layers to the map
	Layer* backgroundLayer = new Layer(map, "Background", 1.0f, true, false);
	map->addLayer(Map::BACKGROUND0, backgroundLayer);

	Layer* objectLayer = new Layer(map, "Objects", 1.0f, true, false);
	map->addLayer(Map::MAPLAYER0, objectLayer);
		
	return true;
}

// Deinitialize the game
void deinit(ESContext *esContext)
{
	// Delete map.
	delete map;
}


// Update game
void update(ESContext* ctx, float deltaTime)
{
	// Update map. this will update all GameObjects inside a map layers.
	map->update(deltaTime);
}


// Draw game
void draw(ESContext *esContext)
{
	// Set OpenGL clear color (dark red gray)
	glClearColor(0.3f, 0.1f, 0.1f, 1.0f);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Set screen size to camera.
	map->getCamera()->setScreenSize(esContext->width, esContext->height, 720, 1280.0f / 720.0f);

	// Render map and all of its layers containing GameObjects to screen.
	map->render();
}


int main(int argc, char *argv[])
{
	ESContext esContext;
	esInitContext(&esContext);
	esCreateWindow(&esContext, "YA-Arkanoid", 1280, 720, ES_WINDOW_DEFAULT);

	esRegisterInitFunc(&esContext, init);
	esRegisterDrawFunc(&esContext, draw);
	esRegisterUpdateFunc(&esContext, update);
	esRegisterDeinitFunc(&esContext, deinit);

	esMainLoop(&esContext);
	return 0;
}

