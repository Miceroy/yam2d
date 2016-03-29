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
class SpriteComponent : public Component
{
public:
	/**
	 * Constructor.
	 *
	 * @param gameObjectType Game specific game object type. Useful for for example detecting of "real game object type", like Player or Enemy.
	 * @param texture Texture for SpriteGameObject.
	 */
	SpriteComponent(GameObject* owner, Texture* texture)
		: Component(owner, Component::getDefaultProperties())
		, m_sprite(new Sprite(0))
		, m_texture(texture)
		, m_rotation(0.0f)
		, m_scaling(1.0f)
	{
		assert(owner != 0); // Must have owner game object
		if (texture != 0)
		{
			getGameObject()->setSize(float(texture->getWidth()), float(texture->getHeight()));
		}
	}

	virtual ~SpriteComponent() {}

	//void render( Layer* layer);

	Sprite* getSprite() const { return m_sprite.ptr(); }
	Texture* getTexture() const { return m_texture.ptr(); }

	GameObject* getGameObject() { return (GameObject*)getOwner(); }
	const GameObject* getGameObject() const { return (const GameObject*)getOwner(); }

	void setRenderingEnabled(bool enable)
	{
		m_renderingEnabled = enable;
	}

	bool isRenderingEnabled() const
	{
		return m_renderingEnabled;

	}
	void setScaling(float s)
	{
		m_scaling = s;
	}

	float getScaling() const
	{
		return m_scaling;
	}

	void setRotation(float rotation)
	{
		m_rotation = rotation;
	}

	float getRotation() const
	{
		return m_rotation;
	}

	void setColor(float r, float g, float b, float a = 1.0f)
	{
		m_sprite->setColor(r, g, b, a);
	}

private:
		
	Ref<Sprite>			m_sprite;
	Ref<Texture>		m_texture;
	float m_rotation;
	float m_scaling;
	bool m_renderingEnabled;

	SpriteComponent();
	SpriteComponent(const SpriteComponent& o);
	SpriteComponent& operator=(const SpriteComponent& o);
};


}

#endif




