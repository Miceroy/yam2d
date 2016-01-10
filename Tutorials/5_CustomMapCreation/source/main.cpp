// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Camera class
#include <Camera.h>

#include <Tileset.h>
#include <Layer.h>
#include <TileComponent.h>


using namespace yam2d;

namespace
{
	// Pointer to TmxMap-object
	TmxMap* map = 0;


	// Component Factory is used to create new components.
	ComponentFactory* componentFactory = 0;

	class CustomComponentFactory : public yam2d::DefaultComponentFactory
	{
	public:
		virtual Component* createNewComponent(const std::string& type, Entity* owner, const yam2d::PropertySet& properties)
		{
			// TODO: Implementation... Use now default implementation instead.
			return DefaultComponentFactory::createNewComponent(type, owner, properties);
		}


		virtual Entity* createNewEntity(ComponentFactory* componentFactory, const std::string& type, Entity* parent, const yam2d::PropertySet& properties)
		{
			// TODO: Implementation... Use now default implementation instead.
			return DefaultComponentFactory::createNewEntity(componentFactory, type, parent, properties);
		}
	};

}


// Initialize the game
bool init ( ESContext *esContext )
{
	// Create new TmxMap object
	map = new TmxMap();
	componentFactory = new CustomComponentFactory();

	// Load map file
	bool okay = map->loadMapFile("level.tmx",componentFactory);

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
	delete componentFactory;
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
	esCreateWindow( &esContext, "Custom map creation", 1280, 720, ES_WINDOW_DEFAULT );

	esRegisterInitFunc( &esContext, init );
	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
    esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
