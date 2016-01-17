// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
#include "PlayerCharacterController.h"
#include "PhysicsBody.h"
#include "PhysicsCollider.h"
#include <TextComponent.h>
// Camera class
#include <Camera.h>
#include <TileComponent.h>
#include <Layer.h>
#include <Input.h>
#include <Box2D\Box2D.h>
#include <MyContactListener.h>

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

		yam2d::Ref<b2World> m_world;
		yam2d::Ref<MyContactListener> m_contactListener;

	public:
		CustomComponentFactory()
			:DefaultComponentFactory()
			, m_playerTexture()
			, m_enemyTexture()
			, m_map(0)
		{
			m_world = new b2World(vec2(0,9.81f));
			m_world->SetAllowSleeping(false);
			m_contactListener = new MyContactListener();
			m_world->SetContactListener(m_contactListener);
		}

		void setCurrentMap(Map* map)
		{
			m_map = map;
		}


		b2World* getPhysicsWorld()
		{
			return m_world;
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
			if ("PlayerSpawn" == type)
			{
				// Create new player.
				GameObject* gameObject = new GameObject(parent, properties);
				//SpriteComponent* spriteComponent = new SpriteComponent(gameObject, m_playerTexture);
				PlayerCharacterController* playerController = new PlayerCharacterController(gameObject);

				float linearDamping = 2.0f;
				float angularDamping = 8.0f;

				// Dynamic body
				
				PhysicsBody* body = new PhysicsBody(gameObject, m_world, linearDamping, angularDamping );
				float density = 1.0f;
				float restitution = 1.0f;
				float friction = 10.0f;
				body->setCircleFixture(0.5f, false,density, restitution, friction);
				gameObject->addComponent(body);
				gameObject->addComponent(componentFactory->createNewComponent("Tile",gameObject,properties));
				gameObject->addComponent(playerController);
				gameObject->setName("Player"); // Set name for player

				return gameObject;
			}
			else if ("DynamicBox" == type)
			{
				GameObject* gameObject = new GameObject(parent, properties);
				float linearDamping = 2.0f;
				float angularDamping = 8.0f;

				// Dynamic body
				PhysicsBody* body = new PhysicsBody(gameObject, m_world, linearDamping, angularDamping);
				float density = 1.0f;
				float restitution = 1.0f;
				float friction = 0.9f;
				vec2 center = yam2d::vec2(0, 0);
				body->setBoxFixture(gameObject->getSizeInTiles(), center, gameObject->getRotation(), false, density, restitution, friction);

				gameObject->addComponent(body);
				gameObject->addComponent(componentFactory->createNewComponent("Tile", gameObject, properties));

				return gameObject;
			}
			else if ("StaticCollisionObject" == type)
			{
				GameObject* gameObject = new GameObject(parent,properties);

				// Static body
				PhysicsBody* body = new PhysicsBody(gameObject, m_world);
				float density = 0.0f;
				float restitution = 1.0f;
				float friction = 1.0f;
				vec2 center = yam2d::vec2(0, 0);
				body->setBoxFixture(gameObject->getSizeInTiles(), center, gameObject->getRotation(), false, density, restitution, friction);

				gameObject->addComponent(body);

				return gameObject;
			}

			// Default functionality.
			Entity* res = DefaultComponentFactory::createNewEntity(componentFactory, type, parent, properties);
			assert(res != 0);
			return res;
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
	
	// Initial camera position
#if 1
	map->getCamera()->setPosition(vec2(
		(0.5f*esContext->width) / map->getTileWidth() - 1.0f,
		map->getHeight() - (0.5f*esContext->height) / map->getTileHeight()));
#else
	map->getCamera()->setPosition(vec2(0));
#endif
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
	float delta = 0.0f;
	float cameraSpeed = 5.0f; // tiles / second
	if (getKeyState(KEY_LEFT))
	{
		delta -= cameraSpeed*deltaTime;
	}
	
	if (getKeyState(KEY_RIGHT))
	{
		delta += cameraSpeed*deltaTime;
	}

	Camera* camera = map->getCamera();
	camera->setPosition(camera->getPosition() + vec2(delta, 0));

	componentFactory->getPhysicsWorld()->Step(deltaTime, 10, 10);
	
	// Update map. this will update all GameObjects inside a map layers.
	map->update(deltaTime);

	// Quit if excape pressed
	if (getKeyState(KEY_ESCAPE))
		esQuitApp(ctx);

}

#if 1
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;
#else
int SCREEN_WIDTH = 400;
int SCREEN_HEIGHT = 400;
#endif

// Draw game
void draw ( ESContext *esContext )
{
	// Set OpenGL clear color (dark gray)
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );

	// Clear the color buffer
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Set screen size to camera.
	map->getCamera()->setScreenSize(esContext->width, esContext->height, (float)SCREEN_HEIGHT, (float)(SCREEN_WIDTH) / (float)SCREEN_HEIGHT);

	// Render map and all of its layers containing GameObjects to screen.
	map->render();
}


int main ( int argc, char *argv[] )
{
	ESContext esContext;
	esInitContext ( &esContext );
	esCreateWindow(&esContext, "Sprite animation", SCREEN_WIDTH, SCREEN_HEIGHT, ES_WINDOW_DEFAULT);

	esRegisterInitFunc( &esContext, init );
	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
    esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
