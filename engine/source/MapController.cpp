// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// http://code.google.com/p/yam2d/
//
// Copyright (c) 2013 Mikko Romppainen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the
// Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "TileComponent.h"
#include "Map.h"
#include <tmx-parser/Tmx.h>
#include "SpriteSheet.h"
#include "Texture.h"
#include "es_assert.h"
#include <vec2.h>
#include "es_util.h"
#include "Text.h"
#include "Layer.h"
#include <Camera.h>
#include <SpriteComponent.h>
#include <SpriteSheetComponent.h>
#include <AnimatedSpriteComponent.h>
#include <TextComponent.h>


namespace yam2d
{



using namespace std;



void Renderer_renderSpriteComponent(SpriteComponent* spriteComponent, Layer* layer);

void Renderer_renderTile(TileComponent* tileComponent, Layer* layer)
{
	if (tileComponent->getTileset() != 0)
	{
		GameObject* gameObject = tileComponent->getGameObject();

		vec2 position = layer->getMap()->tileToDeviceCoordinates(gameObject->getPosition().x, gameObject->getPosition().y);
		tileComponent->getSprite()->setDepth(layer->getDepth());
		tileComponent->getSprite()->setOpacity(layer->getOpacity());
		Tileset* tileset = tileComponent->getTileset();

		Sprite::PixelClip clip = tileset->getSpriteSheet()->getClip(tileComponent->getTileId());
		Texture* tex = tileset->getSpriteSheet()->getTexture();
		tileComponent->getSprite()->setClip(float(tex->getWidth()), float(tex->getHeight()), clip);

		vec2 scale(1.0f);
		if (tileComponent->isFlippedHorizontally())
		{
			scale.x *= -1.0f;
		}

		if (tileComponent->isFlippedVertically())
		{
			scale.y *= -1.0f;
		}

		vec2 p = position;
		//p.x += clip.clipSize.x*0.5f;
		p.y += clip.clipSize.y*0.5f;
		p.x += layer->getMap()->getTileWidth();// *0.5f;
	//	p.x -= 1.5f;
		p.y -= layer->getMap()->getTileHeight();// *0.5f;
		p.x -= tileset->getTileOffsetX();
		p.y -= tileset->getTileOffsetY();

		p.y += layer->getMap()->getTileWidth() * 0.5f;
		p.x -= layer->getMap()->getTileWidth() * 1.0f;

		if (gameObject->getName() == "Ball")
		{
			int i = 0;
			i;
		}

		layer->getBatch()->addSprite(tex, tileComponent->getSprite(), p, gameObject->getRotation(), scale, vec2(0,0));
	}
}


void Renderer_renderSpriteSheet(SpriteSheetComponent* spriteSheetComponent, Layer* layer)
{
	Texture* tex = spriteSheetComponent->getSpriteSheet()->getTexture();
	Sprite::PixelClip clip = spriteSheetComponent->getSpriteSheet()->getClip(spriteSheetComponent->getIdInSpriteSheet());
	spriteSheetComponent->getSprite()->setClip(float(tex->getWidth()), float(tex->getHeight()), clip);
	Renderer_renderSpriteComponent(spriteSheetComponent,layer);
}


void Renderer_renderSpriteComponent(SpriteComponent* spriteComponent, Layer* layer)
{
	if (spriteComponent->isRenderingEnabled())
	{
		vec2 position = layer->getMap()->tileToDeviceCoordinates(spriteComponent->getGameObject()->getPosition().x, spriteComponent->getGameObject()->getPosition().y);
		spriteComponent->getSprite()->setDepth(layer->getDepth());
		spriteComponent->getSprite()->setOpacity(layer->getOpacity());
		spriteComponent->getSprite()->setScale(spriteComponent->getGameObject()->getSize());

		position.x += layer->getMap()->getTileWidth();// *0.5f;
		position.y -= layer->getMap()->getTileHeight() *0.5f;

		position.y += layer->getMap()->getTileWidth() * 0.5f;
		position.x -= layer->getMap()->getTileHeight() * 1.0f;
		float rotation = spriteComponent->getRotation() + spriteComponent->getGameObject()->getRotation();
		layer->getBatch()->addSprite(spriteComponent->getTexture(), spriteComponent->getSprite(), position, -rotation, vec2(spriteComponent->getScaling()));
	}
}

void Renderer_renderSprite(Sprite* sprite, Layer* layer)
{
	vec2 position = layer->getMap()->tileToDeviceCoordinates(sprite->getGameObject()->getPosition().x, sprite->getGameObject()->getPosition().y);
	sprite->setDepth(layer->getDepth());
	sprite->setOpacity(layer->getOpacity());
	sprite->setScale(sprite->getGameObject()->getSize());

	position.x += layer->getMap()->getTileWidth();// *0.5f;
	position.y -= layer->getMap()->getTileHeight() *0.5f;

	position.y += layer->getMap()->getTileWidth() * 0.5f;
	position.x -= layer->getMap()->getTileHeight() * 1.0f;
	layer->getBatch()->addSprite(0, sprite, position, -sprite->getGameObject()->getRotation(), vec2(1.0f));
}


void Renderer_renderText(Text* textComponent, Layer* layer)
{
	GameObject* go = textComponent->getGameObject();
	vec2 position = layer->getMap()->tileToDeviceCoordinates(go->getPosition().x, go->getPosition().y);
	textComponent->setDepth(layer->getDepth());
	textComponent->setOpacity(layer->getOpacity());

	layer->getBatch()->addText(textComponent->getFont()->getTexture(), textComponent, position, -go->getRotation());
}


void Renderer_renderAnimatedSprite(AnimatedSpriteComponent* animatedSpriteComponent, Layer* layer)
{
	int index = animatedSpriteComponent->getAnimation()->getCurrentClipIndex();
	if( index >= 0 )
	{
		animatedSpriteComponent->setIdInSpriteSheet(index);
	}
	Renderer_renderSpriteSheet(animatedSpriteComponent,layer);
}


void renderCamera(Camera* camera, Layer* layer)
{
//	glClear ( GL_DEPTH_BUFFER_BIT );
	// Set the viewport with tear edges
	esViewportTearEdges(camera->getScreenWidth(), camera->getScreenHeight(), camera->getDesiredAspectRatio());
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float left = -0.5f*camera->getDesiredAspectRatio()*camera->getScreenUnitSize();
	float right = 0.5f*camera->getDesiredAspectRatio()*camera->getScreenUnitSize();
	float bottom = -0.5f*camera->getScreenUnitSize();
	float top = 0.5f*camera->getScreenUnitSize();
	
	//SCREEN_UNIT_SIZE = esContext->width;
	//glOrthof( 2.1f*left, 2.1f*right, 2.1f*bottom, 2.1f*top, 1.0f, -1.0f);
	glOrthof( float(int(left)), float(int(right)), float(int(bottom)), float(int(top)), -float(Map::NUM_LAYERS),0.0f);
//	glOrthof( 2.1f*float(int(left)), 2.1f*float(int(right)), 2.1f*float(int(bottom)), 2.1f*float(int(top)), -float(Map::NUM_LAYERS),0.0f);
//	glOrthof( 1.1f*float(int(left)), 1.1f*float(int(right)), 1.1f*float(int(bottom)), 1.1f*float(int(top)), -float(Map::NUM_LAYERS),0.0f);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	vec2 camPos = layer->getMap()->tileToDeviceCoordinates(camera->getPosition());
	glTranslatef( -camPos.x, -camPos.y, 0);

	float sizeX = float(right-left);//m_desiredAspectRatio * (m_screenUnitSize);
	float sizeY = float(top-bottom);//(m_screenUnitSize);

	vec2 camSizeInTiles = vec2(sizeX,sizeY);
	camSizeInTiles.x /= layer->getMap()->getTileWidth();
	camSizeInTiles.y /= layer->getMap()->getTileHeight();
	camera->setSize(camSizeInTiles);
}

void updateGameObject(GameObject* gameObject, float deltaTime)
{
#if 1
	std::vector<Updatable*> updatebleComponents = gameObject->getComponentsOfInterface<Updatable>();
	for (size_t i = 0; i < updatebleComponents.size(); ++i)
	{
		updatebleComponents[i]->update(deltaTime);
	}
#else
	Updatable* updatebleGameObject = dynamic_cast<Updatable*>(gameObject);
	if ( updatebleGameObject != 0)
	{
		updatebleGameObject->update(deltaTime);
	}
#endif
}

void updateLayer(Layer* layer, float deltaTime)
{
	Layer::GameObjectList& gameObjects = layer->getGameObjects();
	for (size_t i = 0; i<gameObjects.size(); ++i)
	{
		updateGameObject(gameObjects[i],deltaTime);
	}
}

void renderLayerObject(GameObject* gameObject, Layer* layer)
{
#if 1
	{
		std::vector<AnimatedSpriteComponent*> animatedSpriteComponents = gameObject->getComponents<AnimatedSpriteComponent>();
		for (size_t i = 0; i < animatedSpriteComponents.size(); ++i)
		{
			Renderer_renderAnimatedSprite(animatedSpriteComponents[i], layer);
		}
	}

	{

		std::vector<SpriteSheetComponent*> spriteSheetComponents = gameObject->getComponents<SpriteSheetComponent>();
		for (size_t i = 0; i < spriteSheetComponents.size(); ++i)
		{
			Renderer_renderSpriteSheet(spriteSheetComponents[i], layer);
		}
	}

	{
		std::vector<SpriteComponent*> spriteComponents = gameObject->getComponents<SpriteComponent>();
		for (size_t i = 0; i < spriteComponents.size(); ++i)
		{
			Renderer_renderSpriteComponent(spriteComponents[i], layer);
		}
	}

	{
		std::vector<Sprite*> sprites = gameObject->getComponents<Sprite>();
		for (size_t i = 0; i < sprites.size(); ++i)
		{
			Renderer_renderSprite(sprites[i], layer);
		}
	}

	{
		std::vector<TileComponent*> tileComponents = gameObject->getComponents<TileComponent>();
		for (size_t i = 0; i < tileComponents.size(); ++i)
		{
			Renderer_renderTile(tileComponents[i], layer);
		}
	}

	{
		std::vector < Text*> texts = gameObject->getComponents<Text>();
		for (size_t i = 0; i < texts.size(); ++i)
		{
			Renderer_renderText(texts[i], layer);
		}
	}

	{
		std::vector < TextComponent*> texts = gameObject->getComponents<TextComponent>();
		for (size_t i = 0; i < texts.size(); ++i)
		{
			Renderer_renderText(texts[i]->getText(), layer);
		}
	}
#else
	{
		AnimatedSpriteComponent* go = dynamic_cast<AnimatedSpriteComponent*>(gameObject);
		if (go != 0)
		{
			Renderer_renderAnimatedSprite(go, layer);
			return;
		}
	}

	{
		SpriteSheetComponent* go = dynamic_cast<SpriteSheetComponent*>(gameObject);
		if (go != 0)
		{
			Renderer_renderSpriteSheet(go, layer);
			return;
		}
	}

	{
		SpriteComponent* go = dynamic_cast<SpriteComponent*>(gameObject);
		if (go != 0)
		{
			Renderer_renderSprite(go, layer);
			return;
		}
	}

	{
		TileComponent* go = dynamic_cast<TileComponent*>(gameObject);
		if (go != 0)
		{
			Renderer_renderTile(go, layer);
			return;
		}
	}

	{
		Text* go = dynamic_cast<Text*>(gameObject);
		if (go != 0)
		{
			Renderer_renderText(go, layer);
			return;
		}
	}

	{
		TextComponent* go = dynamic_cast<TextComponent*>(gameObject);
		if (go != 0)
		{
			Renderer_renderText(go->getText(), layer);
			return;
		}
	}

	

#endif
}


}

