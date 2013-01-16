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
#ifndef TILE_H_
#define TILE_H_

#include <vector>
#include <string>
#include <map>
#include "SpriteBatch.h"

#include "GameObject.h"
#include "PropertySet.h"
#include "SpriteSheet.h"
#include "SpriteAnimation.h"
#include "Map.h"
#include "Tileset.h"

namespace yam2d
{
	

enum GameObjectType
{
	SingleSpriteGameObjectType,
	SingleTextGameObjectType,
	TileGameObjectType
};

class Tileset;
class Map;
class SpriteSheet;

/**
 * Class for Tile.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class Tile : public GameObject
{
public:
	// Default constructor.
	Tile(int gameObjectType, const PropertySet& properties, const vec2& position, Tileset* tileset, unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally
		, float levelTileSizeX, float levelTilesizeY )
		: GameObject(gameObjectType)
		, m_sprite(new Sprite())
		, m_tileset(tileset)
		, m_id(id)
		, m_flippedHorizontally(flippedHorizontally)
		, m_flippedVertically(flippedVertically)
		, m_flippedDiagonally(flippedDiagonally)
	{
		GameObject::setPosition(position);
		GameObject::setOrigin( vec2(tileset->getTileOffsetX(),tileset->getTileOffsetY()) );

		Sprite::PixelClip clip = tileset->getSpriteSheet()->getClip(getTileId());
		setSize( vec2(clip.clipSize.x/levelTileSizeX,clip.clipSize.y/levelTilesizeY) );
		//Texture* tex = tileset->getSpriteSheet()->getTexture();
		//m_sprite->setClip(float(tex->getWidth()), float(tex->getHeight()), clip);

	}

	virtual ~Tile() {}

	Tileset* getTileset() const { return m_tileset; }
	unsigned getTileId() const { return m_id; }
	bool isFlippedHorizontally() const { return m_flippedHorizontally; }
	bool isFlippedVertically() const { return m_flippedVertically; }
	bool isFlippedDiagonally() const { return m_flippedDiagonally; }

	void render(Layer* layer);

private:
	Sprite* m_sprite;
	Tileset* m_tileset;				// Tileset
	unsigned m_id;					// Id.
	bool m_flippedHorizontally;		// True when the tile should be drawn flipped horizontally.
	bool m_flippedVertically;		// True when the tile should be drawn flipped vertically.
	bool m_flippedDiagonally;		// True when the tile should be drawn flipped diagonally.

	Tile();
	Tile(const Tile& o);
	Tile& operator=(const Tile& o);
};


/**
 * Class for SpriteGameObject.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class SpriteGameObject : public GameObject
{
public:
	// Default constructor.
	SpriteGameObject(int gameObjectType, Texture* texture )
		: GameObject(gameObjectType)
		, m_sprite(new Sprite())
		, m_texture(texture)
	{
	}

	virtual ~SpriteGameObject() {}

	void render( Layer* layer);

	Sprite* getSprite() const { return m_sprite; }
private:
		
	Sprite*			m_sprite;
	Texture*		m_texture;

	SpriteGameObject();
	SpriteGameObject(const SpriteGameObject& o);
	SpriteGameObject& operator=(const SpriteGameObject& o);
};

/**
 * Class for TextGameObject.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class TextGameObject : public GameObject
{
public:
	// Default constructor.
	TextGameObject(int gameObjectType, Text* text )
		: GameObject(gameObjectType)
		, m_text(text)
	{
	}

	virtual ~TextGameObject() {}

	void render( Layer* layer);

private:
	Text*		m_text;

	TextGameObject();
	TextGameObject(const TextGameObject& o);
	TextGameObject& operator=(const TextGameObject& o);
};

/**
 * Class for SpriteSheetGameObject.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class SpriteSheetGameObject : public SpriteGameObject
{
public:
	// Default constructor.
	SpriteSheetGameObject(int gameObjectType, SpriteSheet* spriteSheet, int idInSpriteSheet )
		: SpriteGameObject(gameObjectType,spriteSheet->getTexture())
		, m_spriteSheet(spriteSheet)
		, m_id(idInSpriteSheet)
	{
	}

	virtual ~SpriteSheetGameObject() {}

	void setIdInSpriteSheet(int idInSpriteSheet)
	{
		m_id = idInSpriteSheet;
	}

	void render(Layer* layer);

private:
	SpriteSheet*	m_spriteSheet;	// Sprite sheet
	unsigned		m_id;			// Id at sprite sheet

	SpriteSheetGameObject();
	SpriteSheetGameObject(const SpriteSheetGameObject& o);
	SpriteSheetGameObject& operator=(const SpriteSheetGameObject& o);
};

/**
 * Class for AnimatedSpriteGameObject.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class AnimatedSpriteGameObject : public SpriteSheetGameObject
{
public:
	// Default constructor.
	AnimatedSpriteGameObject(int gameObjectType, SpriteSheet* spriteSheet )
		: SpriteSheetGameObject(gameObjectType,spriteSheet,0)
		, m_animation(new SpriteAnimation())
	{
	}

	virtual ~AnimatedSpriteGameObject() {}

	void addAnimation( int animationId, const SpriteAnimation::SpriteAnimationClip& clip )
	{
		m_animation->addAnimation(animationId,clip);
	}

	void setActiveAnimation(int animationId)
	{
		m_animation->setActiveAnimation(animationId);
	}

	int getActiveAnimation()
	{
		return m_animation->getActiveAnimation();
	}

	void update(float deltaTime)
	{
		m_animation->update(deltaTime);
	}

	void render( Layer* layer);

private:
	SpriteAnimation* m_animation;

	AnimatedSpriteGameObject();
	AnimatedSpriteGameObject(const AnimatedSpriteGameObject& o);
	AnimatedSpriteGameObject& operator=(const AnimatedSpriteGameObject& o);
};

/**
 * Class for Camera.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class Camera : public GameObject
{
public:
	// Default constructor.
	Camera(int gameObjectType)
		: GameObject(gameObjectType)
		, m_screenWidth(0)
		, m_screenHeight(0)
		, m_desiredAspectRatio(0.0f)
	{
	}

	virtual ~Camera() {}

	/*

	void update(float deltaTime)
	{
		m_animation->update(deltaTime);
	}
	*/
	void setScreenSize(int screenWidth, int screenHeight )
	{
		setScreenSize(screenWidth,screenHeight,float(screenWidth)/float(screenHeight),float(screenHeight));
	}

	void setScreenSize(int screenWidth, int screenHeight, float desiredAspectRatio, float screenUnitSize )
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
		m_desiredAspectRatio = desiredAspectRatio;
		m_screenUnitSize = screenUnitSize;
	}

	virtual void render( Layer* layer);
	
private:
	int m_screenWidth;
	int m_screenHeight;
	float m_desiredAspectRatio;
	float m_screenUnitSize;
//	SpriteAnimation* m_animation;

	Camera();
	Camera(const Camera& o);
	Camera& operator=(const Camera& o);
};

}

#endif




