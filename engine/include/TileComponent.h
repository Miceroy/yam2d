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
#include "Text.h"
#include "Texture.h"

namespace yam2d
{
	

class Tileset;


/**
 * Class for Tile.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class TileComponent : public Component
{
public:
	// Default constructor.
	TileComponent(Entity* owner, /*int gameObjectType, const vec2& position,*//* Tileset* tileset,*/ unsigned id, bool flippedHorizontally, bool flippedVertically, bool flippedDiagonally)
		: Component(owner,Component::getDefaultProperties())
		, m_sprite(new Sprite(0))
		, m_tileset(0)
		, m_id(id)
		, m_flippedHorizontally(flippedHorizontally)
		, m_flippedVertically(flippedVertically)
		, m_flippedDiagonally(flippedDiagonally)
	{
	//	getGameObject()->setPosition(position);
	}

	virtual ~TileComponent() {}

	void setTileSet(Tileset* tileset/*, float levelTileSizeX, float levelTilesizeY, bool setOffset = true*/)
	{
		m_tileset = tileset;
		Sprite::PixelClip clip = tileset->getSpriteSheet()->getClip(getTileId());

		//if (setOffset)
		//{
			// Set origin to be center of the tile
		//	vec2 offset(0);

		//	offset.x += 0.5f*clip.clipSize.x / levelTileSizeX;
		//	offset.y -= 0.5f*clip.clipSize.y / levelTilesizeY;

		//	offset.x -= 0.5f;
		//	offset.y += 0.5f;

		//	getGameObject()->setOffset(offset);
		//}
		//else
		//{
			// Set origin to be center of the tile
		//	vec2 offset(0);

		//	offset.x += 0.5f*clip.clipSize.x / levelTileSizeX;
			
		//	offset.y -= 0.5f*clip.clipSize.y / levelTilesizeY;
		//	offset.x -= 1.5f;
			//offset.y += 0.5f;

		//	getGameObject()->setOffset(offset);
		//}

		getGameObject()->setSize(vec2(float(clip.clipSize.x), float(clip.clipSize.y)));
	}

	Tileset* getTileset() const { return m_tileset.ptr(); }
	unsigned getTileId() const { return m_id; }
	bool isFlippedHorizontally() const { return m_flippedHorizontally; }
	bool isFlippedVertically() const { return m_flippedVertically; }
	bool isFlippedDiagonally() const { return m_flippedDiagonally; }

	//void setOffset( const vec2& offset ) { m_offset = offset; recalcExtens(); }
	//const vec2& getOffset() const { return m_offset; }

	//void render(Layer* layer);
	Sprite* getSprite() const { return m_sprite.ptr(); }
	GameObject* getGameObject() { return (GameObject*)getOwner(); }
	const GameObject* getGameObject() const { return (const GameObject*)getOwner(); }
private:
	Ref<Sprite> m_sprite;
	Ref<Tileset> m_tileset;				// Tileset
	unsigned m_id;					// Id.
	bool m_flippedHorizontally;		// True when the tile should be drawn flipped horizontally.
	bool m_flippedVertically;		// True when the tile should be drawn flipped vertically.
	bool m_flippedDiagonally;		// True when the tile should be drawn flipped diagonally.

	TileComponent();
	TileComponent(const TileComponent& o);
	TileComponent& operator=(const TileComponent& o);
};



}

#endif




