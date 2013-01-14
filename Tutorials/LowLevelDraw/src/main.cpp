// Include OpenGL ES Engine utils
#include <es_util.h>
#include <SpriteBatch.h>
#include <SpriteSheet.h>
#include <Text.h>

float count = 0.0f;

using namespace yam2d;

namespace
{
	// SpriteBatchGroup is used to draw Sprites and text.
	SpriteBatchGroup* batch = 0;

	// Texture and sprite
	Texture* openGLTexture = 0;
	Sprite* sprite = 0;

	// Font and text.
	SpriteSheet* font = 0;
	Texture* fontTexture = 0;
	Text* text = 0;
}

// Initialize the game
bool init ( ESContext *esContext )
{
	// Create new sprite batch group. This is deleted at deinit.
	batch = new SpriteBatchGroup();
	
	// Load OpenGL logo.
	openGLTexture = new Texture("opengl-logo.png");
	
	// Create new sprite.
	sprite = new Sprite();
	
	// Load font texture. Made with font creation tool like bitmap font builder
	fontTexture = new Texture("Fixedsys_24_Bold.png");

	// Create font clip areas (sprite sheet), from dat file and texture.
	SpriteSheet* font = SpriteSheet::autoFindFontFromTexture(fontTexture,"Fixedsys_24_Bold.dat");

	// Create new text-object
	text = new Text(font);

	return true;
}

// Deinitialize the game
void deinit ( ESContext *esContext )
{
	// Delete sprite batch group.
	delete batch;
}


// Update game
void update( ESContext* ctx, float deltaTime )
{
	count += deltaTime;

	text->setText("Hello world!!");

	// Clear sprite before add new dynamic sprites.
	batch->clear();

	// Add sprite. We need to scale this a bit (100 times).
	batch->addSprite(openGLTexture, sprite, vec2(0,0), count, vec2(100) );

	// Add text.
	batch->addText(fontTexture, text, vec2(-400,300), 0);
}


// Draw game
void draw ( ESContext *esContext )
{
	// Set OpenGL clear color
	glClearColor( 0.1f, 0.1f, 0.1f, 0.0f );

	// Clear the color buffer
	glClear ( GL_COLOR_BUFFER_BIT );
	
	// Set the viewport to be full window area.
	glViewport( 0, 0, esContext->width, esContext->height );
	
	// Set projection to identity
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
		
	// Calculate half screewn size
	float left = -0.5f*esContext->width;
	float right = 0.5f*esContext->width;
	float bottom = -0.5f*esContext->height;
	float top = 0.5f*esContext->height;
	
	// Set orthogonal projection
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

	// Swap to back buffer
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}


int main ( int argc, char *argv[] )
{
	ESContext esContext;
	esInitContext ( &esContext );
	esCreateWindow( &esContext, "Hello Triangle", 1280, 720, ES_WINDOW_DEFAULT );
   
	if ( !init ( &esContext ) )
		return 0;

	esRegisterDrawFunc( &esContext, draw );
	esRegisterUpdateFunc( &esContext, update );
	esRegisterDeinitFunc( &esContext, deinit);

	esMainLoop ( &esContext );
	return 0;
}
