// Include OpenGL ES Engine utils
#include <es_util.h>
#include <SpriteBatch.h>
#include <SpriteSheet.h>
#include <Text.h>
#include <Texture.h>

float count = 0.0f;

using namespace yam2d;

namespace
{
	// SpriteBatchGroup is used to draw Sprites and Texts.
	SpriteBatchGroup* batch = 0;

	// Texture (loaded bitmap)
	Texture* openGLTexture = 0;
	// Sprite (specifies rendering paremeters for region of texture to be rendered to screen)
	Sprite* sprite = 0;

	// Font is series of clipping regions (SpriteSheet), which specifies font and character set.
	SpriteSheet* font = 0;
	// Texture to be used when rendering text
	Texture* fontTexture = 0;
	// Text object contain text to be rendered to the screen using given font.
	Text* text = 0;
}

// Initialize the game
bool init ( ESContext *esContext )
{
	esLogMessage(__FUNCTION__);
	int cc = 0;
	esLogMessage("Init... %d", cc++);
	// Create new sprite batch group. This must be deleted at deinit.
	batch = new SpriteBatchGroup();
	
	esLogMessage("Init... %d", cc++);
	// Load OpenGL logo to be used as texture for sprite.
	openGLTexture = new Texture("opengl-logo.png");
	
	esLogMessage("Init... %d", cc++);
	// Create new sprite, with default parameters.
	sprite = new Sprite(0);
	
	esLogMessage("Init... %d", cc++);
	// Load font texture. Made with font creation tool like bitmap font builder.
	fontTexture = new Texture("Fixedsys_24_Bold.png");

	esLogMessage("Init... %d", cc++);
	// Create font clip areas (sprite sheet), from dat file and texture. Dat-file is made with bitmap font builder.
	SpriteSheet* font = SpriteSheet::autoFindFontFromTexture(fontTexture,"Fixedsys_24_Bold.dat");

	esLogMessage("Init... %d", cc++);
	// Create new text-object
	text = new Text(0,font);

	esLogMessage("Init... Done");
	return true;
}

// Deinitialize the game
void deinit ( ESContext *esContext )
{
	esLogMessage(__FUNCTION__);
	// Delete sprite batch group.
	delete sprite;
	delete text;
	delete batch;
}


// Update game
void update( ESContext* ctx, float deltaTime )
{
	//esLogMessage(__FUNCTION__);
	// Update total time counter.
	count += deltaTime;

	// Set text.
	text->setText("Hello world!!");

	// Clear sprite before add new dynamic sprites.
	batch->clear();

	// Add sprite. Rotate it according to total time. We need also scale font a bit (100 times, so the sprite is 100x100 pixels).
	batch->addSprite(openGLTexture, sprite, vec2(0,0), count, vec2(100) );

	// Add text to position -400,300
	batch->addText(fontTexture, text, vec2(-ctx->width / 3.0f, ctx->height / 3.0f), 0.0f);
}


// Draw game
void draw ( ESContext *esContext )
{
	// Set OpenGL clear color
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );

	// Clear the color buffer
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	// Set the viewport to be full window area.
	glViewport( 0, 0, esContext->width, esContext->height );
	
	// Set projection to identity
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
		
	// Calculate half screen size
	float left = -0.5f*esContext->width;
	float right = 0.5f*esContext->width;
	float bottom = -0.5f*esContext->height;
	float top = 0.5f*esContext->height;
	
	// Set OpenGL orthogonal projection for screen size <esContext->width,esContext->height>
	glOrthof( float(int(left)), float(int(right)), float(int(bottom)), float(int(top)), -1.0, 1.0f);
	
	// Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// Enable alpha blending (transparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	// Set model view matrix to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw batched objects to screen.
	batch->render();
}


int main ( int argc, char *argv[] )
{
		esLogMessage("Main...");
		ESContext esContext;
		esInitContext ( &esContext );
		esCreateWindow( &esContext, "Hello Triangle", 1280, 720, ES_WINDOW_DEFAULT );
  	//	esCreateWindow( &esContext, "Hello Triangle", 1920, 1080, ES_WINDOW_DEFAULT );
   
		esRegisterInitFunc( &esContext, init );
		esRegisterDrawFunc( &esContext, draw );
		esRegisterUpdateFunc( &esContext, update );
		esRegisterDeinitFunc( &esContext, deinit);

		esMainLoop ( &esContext );

	return 0;
}
