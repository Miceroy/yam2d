// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Camera class
#include <Camera.h>
#include <AnimatedSpriteComponent.h>
#include <TextComponent.h>
#include <Layer.h>
#include <Input.h>

using namespace yam2d;

namespace
{
	// Pointer to TmxMap-object
	TmxMap* map = 0;
	ComponentFactory* componentFactory = 0;

	GameObject* explosion = 0;
	GameObject* snipe = 0;

	// We need to use Ref-class (autopointer), so that we dont lose our object references when explosion is destroyed from the map.
	Ref<Texture> texture = 0;
	Ref<SpriteSheet> spriteSheet = 0; 

	void createNewExplosion()
	{
		assert( explosion == 0 ); // Only one explosion is allowed
		explosion = new GameObject(0, 0);
		AnimatedSpriteComponent* animatedSprite = new AnimatedSpriteComponent(explosion, spriteSheet);
		explosion->addComponent(animatedSprite);
		explosion->setSize(100, 50);
		std::vector<int> indices;
		indices.resize(spriteSheet->getClipCount());
		for( size_t i=0; i<indices.size(); ++i )
		{
			indices[i] = i;
		}
		explosion->setName("Explosion");
		animatedSprite->addAnimation(0, SpriteAnimation::SpriteAnimationClip(indices, 30.0f, 1.0f, false));
		animatedSprite->setActiveAnimation(0);
		explosion->setPosition(vec2(map->getWidth() / 2.0f - 0.5f, map->getHeight() / 2.0f - 0.5f));
		map->getLayer("GameObjects")->addGameObject(explosion);
	}


	GameObject* createTextGameObject(const std::string& t)
	{
		// Load font texture. Made with font creation tool like bitmap font builder.
		Texture* fontTexture = new Texture("Fixedsys_24_Bold.png");

		// Create font clip areas (sprite sheet), from dat file and texture. Dat-file is made with bitmap font builder.
		SpriteSheet* font = SpriteSheet::autoFindFontFromTexture(fontTexture,"Fixedsys_24_Bold.dat");

		GameObject* gameObject = new GameObject(0, 0);
		TextComponent* textComponent = new TextComponent(gameObject, font);
		gameObject->addComponent(textComponent);
		textComponent->getText()->setText(t);
		textComponent->getText()->setColor(60, 0, 190);
		
		return gameObject;
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

	// Move gameobject to middle of map.
	vec2 cameraPos = vec2(map->getWidth()/2.0f - 0.5f, map->getHeight()/2.0f - 0.5f);
	map->getCamera()->setPosition( cameraPos );

	
	texture = new Texture("snipe.png");
	texture->setTransparentColor(0xff,0x00,0xff);
	spriteSheet = SpriteSheet::generateSpriteSheet(texture,32,32,0,0,0,0); 
	snipe = new GameObject(0, 0);
	snipe->addComponent( new AnimatedSpriteComponent(snipe, spriteSheet));
	snipe->setSize(32,32);
	snipe->setName("Snipe");

	// Add 4 animations
	int numClipsPerAnimation = 8;

	float animationFrameRate = 20.0f;
	for( int i=0; i<4; ++i )
	{
		std::vector<int> indices;
		indices.resize(numClipsPerAnimation);
		for( size_t j=0; j<indices.size(); ++j )
		{
			indices[j] = numClipsPerAnimation*i + j;
		}

		// Add looping animation.
		snipe->getComponent<AnimatedSpriteComponent>()->addAnimation(i, SpriteAnimation::SpriteAnimationClip(indices, animationFrameRate, 1.0f, true));
	}
	// Set animation at index 0 to active
	snipe->getComponent<AnimatedSpriteComponent>()->setActiveAnimation(0);
	// Set snipe position.
	snipe->setPosition( cameraPos + vec2(5,0) );
	map->getLayer("GameObjects")->addGameObject(snipe);
	
	// Preload texture and sprite sheet for explosion, so we do not need to load these every time, when new explosion is created.	
	texture = new Texture("explosion.png");
	spriteSheet = SpriteSheet::autoFindSpriteSheetFromTexture(texture);
	esLogMessage("Found %d sprite clips", spriteSheet->getClipCount() ); 
	// Create first explosion
	createNewExplosion();

	// Create two text lines.
	GameObject* text1 = createTextGameObject("Press ENTER to create new explosion");
	text1->setPosition(0.5f*(text1->getComponent<TextComponent>()->getText()->getWidth()/map->getTileWidth()),0);
	map->getLayer("GameObjects")->addGameObject(text1);

	GameObject* text2 = createTextGameObject("Press SPACE to change animation of Snipe-object");
	text2->setPosition(0.5f*(text2->getComponent<TextComponent>()->getText()->getWidth() / map->getTileWidth()), 1);
	map->getLayer("GameObjects")->addGameObject(text2);
	
	return true;
}

// Deinitialize the game
void deinit ( ESContext *esContext )
{
	// We need to 'delete' these Refs/Autopointers here.
	texture = 0;
	spriteSheet = 0; 

	// Delete map.
	delete map;
	delete componentFactory;
}


// Update game
void update( ESContext* ctx, float deltaTime )
{
	// If explosion does exist and it is finished, delete explosion.
	if ((explosion != 0) && explosion->getComponent<AnimatedSpriteComponent>()->isFinished())
	{
		map->getLayer("GameObjects")->deleteGameObject(explosion);
		explosion = 0;
	}
	
	// If explosion does not exist and return key is down, create new explosion.
	if( (explosion == 0) && getKeyState(KEY_RETURN) )
	{
		createNewExplosion();
	}
	

	bool spaceKeyReleased = false;
	{
		// If previously pressed and now not pressed, key has released
		static bool prevSpacePressed = false;	
		if( prevSpacePressed && !getKeyState(KEY_SPACE) )
			spaceKeyReleased = true;
		
		// Store this frame key info to be used next frame
		if( getKeyState(KEY_SPACE) )
			prevSpacePressed = true;
		else
			prevSpacePressed = false;
	}

	// If key released select next animation id to play
	if(spaceKeyReleased)
	{
		AnimatedSpriteComponent* snipeAnimation = snipe->getComponent<AnimatedSpriteComponent>();
		int nextAnimation = (snipeAnimation->getActiveAnimation() + 1) % snipeAnimation->getNumAnimations();
		snipeAnimation->setActiveAnimation(nextAnimation);
	}

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
