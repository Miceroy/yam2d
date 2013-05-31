// Include OpenGL ES Engine utils
#include <es_util.h>
// Include map class
#include <Map.h>
// Tile
#include <Tile.h>
#include <SpriteGameObject.h>
// Layer
#include <Layer.h>
// Texture
#include <Camera.h>

using namespace yam2d;

namespace
{
	Map* map = 0;


}

SpriteGameObject* createSpriteGameObject(const std::string& bitmapFileName, float sizeX, float sizeY, bool isWhiteTransparentColor = false)
{
	// Load texture to be used as texture for sprite.
	Texture* texture = new Texture(bitmapFileName.c_str());

	// If user wants to create texture which white coros is treated as atransparent color.
	if( isWhiteTransparentColor )
	{
		// Set white to transparent. Here color values are from 0 to 255 (RGB)
		texture->setTransparentColor(255,255,255);
	}

	// Create new sprite GameObject from texture.
	SpriteGameObject* gameObject = new SpriteGameObject(0,texture);

	// Resize the sprite to be correct size
	gameObject->setSize(sizeX, sizeY);

	return gameObject;
}

SpriteGameObject* createSpriteGameObject(const std::string& bitmapFileName, float sizeX, float sizeY, int clipStartX, int clipStartY, int clipSizeX, int clipSizeY, bool isWhiteTransparentColor = false )
{
	// Load texture to be used as texture for sprite.
	Texture* texture = new Texture(bitmapFileName.c_str());
	
	// If user wants to create texture which white coros is treated as atransparent color.
	if( isWhiteTransparentColor )
	{
		// Set white to transparent. Here color values are from 0 to 255 (RGB)
		texture->setTransparentColor(255,255,255);
	}

	// Create new sprite GameObject from texture.
	SpriteGameObject* gameObject = new SpriteGameObject(0,texture);

	// Resize the sprite to be correct size
	gameObject->setSize(sizeX, sizeY);

	// Specify clip area by start point and size in pixels
	Sprite::PixelClip clip;
	clip.topLeft.x = clipStartX;
	clip.topLeft.y = clipStartY;
	clip.clipSize.x = clipSizeX;
	clip.clipSize.y = clipSizeY;

	// Set pixel clip for sprite
	gameObject->getSprite()->setClip(float(texture->getWidth()), float(texture->getHeight()), clip );

	return gameObject;
}



// Initialize the game
bool init ( ESContext *esContext )
{
	// Level tile size
	vec2 tileSize(64,64);

	// Create new map, which tile width == tile height == 32 pixels/tile
	map = new Map(tileSize.x,tileSize.y);
		
	// Create new layer, which is attached to created map. 
	// Layer name is "Background", it is fully opaque (i.e. not transparent or semi-transparent)
	// Layer is visible (it is rendered to screen). And it is dynamic, it is automatically batched 
	// on each frame, so GameObjects can move between frames.
	Layer* backgroundLayer = new Layer(map, "Background", 1.0f, true, false);
	
	// Add created layer to be at BACKGROUND0-layer (depth/drawing order of the layer)
	map->addLayer(Map::BACKGROUND0, backgroundLayer );

	// Create new sprite GameObject from texture (background sprite) size is same than screen size.
	SpriteGameObject* backgroundGameObject = createSpriteGameObject("cloudsbackground.png", 1280.0f, 720.0f);

	// Set some color to sprite, here color values are from 0.0 to 1.0 (RGB or RGBA)
	backgroundGameObject->getSprite()->setColor(1.0f, 0.9f, 0.9f);

	// Add GameObject to background layer.
	backgroundLayer->addGameObject(backgroundGameObject);

	// Create another layer, which is attached to created map. 
	// Layer name is "Objects", it is fully opaque (i.e. not transparent or semi-transparent)
	// Layer is visible (it is rendered to screen). And it is dynamic, it is automatically batched 
	// on each frame, so GameObjects can move between frames.
	Layer* objectLayer = new Layer(map, "Objects", 1.0f, true, false);
	
	// Add created layer to be at MAPLAYER0-layer (depth/drawing order of the layer)
	map->addLayer(Map::MAPLAYER0, objectLayer );

	// Create new ball game object (1x1 tiles), clip it from texture position <0,0> - <64,64>, white color shall be transparent
	SpriteGameObject* ballGameObject = createSpriteGameObject("objects.png", tileSize.x, tileSize.y, 0, 0, 64, 64, true);
	// Add ball to level
	objectLayer->addGameObject(ballGameObject);
	// Set position
	ballGameObject->setPosition(vec2(-1,0));

	// Create new square game object (1x1 tiles), clip it from texture position <192,0> - <64,64>
	SpriteGameObject* square = createSpriteGameObject("objects.png", tileSize.x, tileSize.y, 192, 0, 64, 64);
	// Add square to level
	objectLayer->addGameObject(square);
	// Set position
	square->setPosition(vec2(1,0));

	// Create floor
	for( int x=-4; x<5; ++x )
	{
		// Create new floor game object (2x1 tiles), clip it from texture position <64,0> - <128,64>
		SpriteGameObject* square = createSpriteGameObject("objects.png", tileSize.x*2.0f,tileSize.y, 64, 0, 128, 64);
		// Add to level
		objectLayer->addGameObject(square);
		// Set position
		square->setPosition( vec2(x*2.0f, 5.0f));
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
	glClear ( GL_COLOR_BUFFER_BIT|GL_COLOR_BUFFER_BIT );

	// Set screen size to camera.
	map->getCamera()->setScreenSize(esContext->width,esContext->height, 720, 1280.0f/720.0f); 

	// Render map and all of its layers containing GameObjects to screen.
	map->render();
}


int main ( int argc, char *argv[] )
{
	ESContext esContext;
	esInitContext ( &esContext );
	esCreateWindow( &esContext, "Simple map example", 1280, 720, ES_WINDOW_DEFAULT );
   
	esRegisterInitFunc( &esContext, init );
	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
    esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
