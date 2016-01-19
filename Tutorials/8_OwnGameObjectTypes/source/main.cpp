// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
#include "PlayerDriveController.h"
#include "WaypointController.h"
#include "SpatialAnimationController.h"
#include <TextComponent.h>
// Camera class
#include <Camera.h>
#include <TileComponent.h>
#include <Layer.h>
#include <Input.h>

using namespace yam2d;



namespace
{
	// Forward declaration of CustomComponentFactory
	class CustomComponentFactory;

	// Pointer to TmxMap-object
	TmxMap* map = 0;
	CustomComponentFactory* componentFactory = 0;


	class CustomComponentFactory : public yam2d::DefaultComponentFactory
	{
	private:
		yam2d::Ref<Texture> m_playerTexture; 
		yam2d::Ref<Texture> m_enemyTexture;
		Map* m_map; // HACK. Player to set for each enemy

	public:
		CustomComponentFactory()
			:DefaultComponentFactory()
			, m_playerTexture()
			, m_enemyTexture()
			, m_map(0)
		{
			// Preload textures.

			m_playerTexture = new Texture("red_triangle.png");
			// We have pink background in red_triangle-png.
			m_playerTexture->setTransparentColor(255, 0, 255);

			// Load texture.
			m_enemyTexture = new Texture("blue_triangle.png");
			// We have pink background in blue_triangle-png.
			m_enemyTexture->setTransparentColor(255, 0, 255);
		}

		void setCurrentMap(Map* map)
		{
			m_map = map;
		}

		virtual ~CustomComponentFactory()
		{
		}

		virtual Component* createNewComponent(const std::string& type, Entity* owner, const yam2d::PropertySet& properties)
		{
			// TODO: Implementation... Use now default implementation instead.
			return DefaultComponentFactory::createNewComponent(type, owner, properties);
		}


		virtual Entity* createNewEntity(ComponentFactory* componentFactory, const std::string& type, Entity* parent, const yam2d::PropertySet& properties)
		{
			if ("Player" == type)
			{
				// Create new player.
				GameObject* gameObject = new GameObject(0, 0);
				SpriteComponent* spriteComponent = new SpriteComponent(gameObject, m_playerTexture);
				PlayerDriveController* playerController = new PlayerDriveController(gameObject);
				gameObject->addComponent(spriteComponent);
				gameObject->addComponent(playerController);
				gameObject->setPosition(15, 5);
				gameObject->setName("Player"); // Set name for player
				return gameObject;
			}
			else if ("AnimatedEnemy" == type)
			{
				// Create new animated enemy.
				GameObject* animatedEnemyGameObject = new GameObject(0, 0);
				SpriteComponent* spriteComponent = new SpriteComponent(animatedEnemyGameObject, m_enemyTexture);
				GameObject* player = m_map->findGameObjectByName("Player");
				assert(player != 0); // player not found!
				SpatialAnimationController* animationCtrl = new SpatialAnimationController(animatedEnemyGameObject, vec2(2, 2), player);
				animatedEnemyGameObject->addComponent(spriteComponent);
				animatedEnemyGameObject->addComponent(animationCtrl);
				// Add it to GameObjects-layer.
				return animatedEnemyGameObject;
			}
			else if ("WaypointEnemy" == type)
			{
				// Create new waypoint enemy.
				GameObject* enemyGameObject = new GameObject(0, 0);
				SpriteComponent* spriteComponent = new SpriteComponent(enemyGameObject, m_enemyTexture);
				GameObject* player = m_map->findGameObjectByName("Player");
				assert(player != 0); // player not found!
				WaypointController* wpController = new WaypointController(enemyGameObject, player);
				enemyGameObject->addComponent(spriteComponent);
				enemyGameObject->addComponent(wpController);

				// Add some waypoints for enemy.
				std::vector<vec2> enemyWaypoints;
				enemyWaypoints.push_back(vec2(7, 7));
				enemyWaypoints.push_back(vec2(14, 0));
				enemyWaypoints.push_back(vec2(14, 14));
				enemyWaypoints.push_back(vec2(0, 14));
				enemyWaypoints.push_back(vec2(0, 0));
				wpController->setWayoints(enemyWaypoints);
				return enemyGameObject;
			}

			// Default functionality.
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
	componentFactory->setCurrentMap(map);

	// Load map file
	if( !map->loadMapFile("level.tmx", componentFactory) )
		return false;

	// Move gameobject to middle of map.
	vec2 cameraPos = vec2(map->getWidth()/2.0f - 0.5f, map->getHeight()/2.0f - 0.5f);
	map->getCamera()->setPosition(cameraPos);
		
	{
		// Create Player entity
		GameObject* player = (GameObject*)componentFactory->createNewEntity(componentFactory, "Player", 0, yam2d::PropertySet());
		map->getLayer("GameObjects")->addGameObject(player);
	}

	{
		// Create new AnimatedEnemy.
		GameObject* animatedEnemyGameObject = (GameObject*)componentFactory->createNewEntity(componentFactory, "AnimatedEnemy", 0, yam2d::PropertySet());
		// Add it to GameObjects-layer.
		map->getLayer("GameObjects")->addGameObject(animatedEnemyGameObject);
	}

	{
		// Create new WaypointEnemy.
		GameObject* enemyGameObject = (GameObject*)componentFactory->createNewEntity(componentFactory, "WaypointEnemy", 0, yam2d::PropertySet());
		// Add it to GameObjects-layer.
		map->getLayer("GameObjects")->addGameObject(enemyGameObject);
	}
	
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
void update( ESContext* ctx, float deltaTime )
{
	// Update map. this will update all GameObjects inside a map layers.
	map->update(deltaTime);

	// Quit if excape pressed
	if (getKeyState(KEY_ESCAPE))
		esQuitApp(ctx);
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
	esCreateWindow( &esContext, "Sprite animation", 1280, 720, ES_WINDOW_DEFAULT );

	esRegisterInitFunc( &esContext, init );
	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
    esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
