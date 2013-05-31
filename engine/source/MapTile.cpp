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

#include "Tile.h"
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
#include <SpriteGameObject.h>
#include <SpriteSheetGameObject.h>
#include <AnimatedSpriteGameObject.h>
#include <TextGameObject.h>


namespace yam2d
{


using namespace std;


void Tile::render(Layer* layer)
{
	if( getTileset() != 0 )
	{
		vec2 position = layer->getMap()->tileToDeviceCoordinates(getPosition().x,getPosition().y);
		m_sprite->setDepth( layer->getDepth() );
		m_sprite->setOpacity( layer->getOpacity() );
		Tileset* tileset = getTileset();

		Sprite::PixelClip clip = tileset->getSpriteSheet()->getClip(getTileId());
		Texture* tex = tileset->getSpriteSheet()->getTexture();
		m_sprite->setClip(float(tex->getWidth()), float(tex->getHeight()), clip);

		vec2 scale(1.0f);
		if( isFlippedHorizontally() )
		{
			scale.x *= -1.0f;
		}

		if( isFlippedVertically() )
		{
			scale.y *= -1.0f;
		}

		vec2 p = position;
		p.x += clip.clipSize.x*0.5f;
		p.y += clip.clipSize.y*0.5f;
		p.x -= layer->getMap()->getTileWidth()*0.5f;
		p.y -= layer->getMap()->getTileHeight()*0.5f;
		p.x -= tileset->getTileOffsetX();
		p.y -= tileset->getTileOffsetY();

	//	p += getOffset();
//		vec2 offset(0.0f);

		layer->getBatch()->addSprite( tex, m_sprite, p, 0, scale );
	}
}


void SpriteSheetGameObject::render(Layer* layer)
{
	Texture* tex = m_spriteSheet->getTexture();
	Sprite::PixelClip clip = m_spriteSheet->getClip(m_id);
	getSprite()->setClip(float(tex->getWidth()), float(tex->getHeight()), clip);
	SpriteGameObject::render(layer);
}


void SpriteGameObject::render(Layer* layer)
{
	vec2 position = layer->getMap()->tileToDeviceCoordinates(getPosition().x,getPosition().y);
	m_sprite->setDepth( layer->getDepth() );
	m_sprite->setOpacity( layer->getOpacity() );
	m_sprite->setScale(getSize());

	layer->getBatch()->addSprite( m_texture, m_sprite, position, -getRotation(), vec2(1.0f) );
}


void TextGameObject::render(Layer* layer)
{
	vec2 position = layer->getMap()->tileToDeviceCoordinates(getPosition().x,getPosition().y);
	m_text->setDepth( layer->getDepth() );
	m_text->setOpacity( layer->getOpacity() );

	layer->getBatch()->addText( m_text->getFont()->getTexture(), m_text, position, -getRotation() );
}


void AnimatedSpriteGameObject::render( Layer* layer)
{
	int index = m_animation->getCurrentClipIndex();
	if( index >= 0 )
	{
		SpriteSheetGameObject::setIdInSpriteSheet( index );
	}
	SpriteSheetGameObject::render(layer);
}


void Camera::render(Layer* layer)
{
//	glClear ( GL_DEPTH_BUFFER_BIT );
	// Set the viewport with tear edges
	esViewportTearEdges(m_screenWidth,m_screenHeight,m_desiredAspectRatio);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float left = -0.5f*m_desiredAspectRatio*m_screenUnitSize;
	float right = 0.5f*m_desiredAspectRatio*m_screenUnitSize;
	float bottom = -0.5f*m_screenUnitSize;
	float top = 0.5f*m_screenUnitSize;
	
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


	vec2 camPos = layer->getMap()->tileToDeviceCoordinates(getPosition());
	glTranslatef( -camPos.x, -camPos.y, 0);

	float sizeX = float(right-left);//m_desiredAspectRatio * (m_screenUnitSize);
	float sizeY = float(top-bottom);//(m_screenUnitSize);

	vec2 camSizeInTiles = vec2(sizeX,sizeY);
	camSizeInTiles.x /= layer->getMap()->getTileWidth();
	camSizeInTiles.y /= layer->getMap()->getTileHeight();
	setSize(camSizeInTiles);
}

}

