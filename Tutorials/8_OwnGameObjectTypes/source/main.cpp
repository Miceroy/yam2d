// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
#include "Player.h"
#include "Enemy.h"
#include "AnimatedEnemy.h"
#include <TextGameObject.h>
// Camera class
#include <Camera.h>
#include <Tile.h>
#include <Layer.h>
#include <Input.h>

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
	if( !map->loadMapFile("level.tmx") )
		return false;

	// Move gameobject to middle of map.
	vec2 cameraPos = vec2(map->getWidth()/2.0f - 0.5f, map->getHeight()/2.0f - 0.5f);
	map->getCamera()->setPosition( cameraPos );

	Player* player;
	// Add player to map layer named "GameObjects".
	{
		// Load texture.
		Texture* playerTexture = new Texture("red_triangle.png");
		// We have pink background in red_triangle-png.
		playerTexture->setTransparentColor(255,0,255);

		// Create new player.
		player = new Player(0,playerTexture);
		player->setPosition(15,5);
		// Add it to GameObjects-layer.
		map->getLayer("GameObjects")->addGameObject(player);
	}

	// Add enemy to map layer named "GameObjects".
	{
		// Load texture.
		Texture* enemyTexture = new Texture("blue_triangle.png");
		// We have pink background in blue_triangle-png.
		enemyTexture->setTransparentColor(255,0,255);

		// Create new animated enemy.
		AnimatedEnemy* animatedEnemy = new AnimatedEnemy(0,enemyTexture, vec2(2,2), player);

		// Add it to GameObjects-layer.
		map->getLayer("GameObjects")->addGameObject(animatedEnemy);
		

		// Create new enemy.
		Enemy* enemy = new Enemy(0,enemyTexture, player);

		// Add it to GameObjects-layer.
		map->getLayer("GameObjects")->addGameObject(enemy);
		
		// Add some waypoints for enemy.
		std::vector<vec2> enemyWaypoints;
		enemyWaypoints.push_back( vec2(7,7) ); 
		enemyWaypoints.push_back( vec2(14, 0) ); 
		enemyWaypoints.push_back( vec2(14,14) ); 
		enemyWaypoints.push_back( vec2(0,14) ); 
		enemyWaypoints.push_back( vec2(0,0) ); 
		enemy->setWayoints(enemyWaypoints);
	}
	
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
	esCreateWindow( &esContext, "Sprite animation", 1280, 720, ES_WINDOW_DEFAULT );

	esRegisterInitFunc( &esContext, init );
	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
    esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
