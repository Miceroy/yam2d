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
#ifndef SPRITEGAMEOBJECT_H_
#define SPRITEGAMEOBJECT_H_

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
#include <Texture.h>

namespace yam2d
{
	

class Tileset;
class Map;
class SpriteSheet;



/**
 * Class for SpriteGameObject.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class SpriteGameObject : public GameObject
{
public:
	/**
	 * Constructor.
	 *
	 * @param gameObjectType Game specific game object type. Useful for for example detecting of "real game object type", like Player or Enemy.
	 * @param texture Texture for SpriteGameObject.
	 */
	SpriteGameObject(int gameObjectType, Texture* texture )
		: GameObject(gameObjectType)
		, m_sprite(new Sprite())
		, m_texture(texture)
	{
		setSize(float(texture->getWidth()),float(texture->getHeight()));
	}

	virtual ~SpriteGameObject() {}

	void render( Layer* layer);

	Sprite* getSprite() const { return m_sprite.ptr(); }
private:
		
	Ref<Sprite>			m_sprite;
	Ref<Texture>		m_texture;

	SpriteGameObject();
	SpriteGameObject(const SpriteGameObject& o);
	SpriteGameObject& operator=(const SpriteGameObject& o);
};


}

#endif




