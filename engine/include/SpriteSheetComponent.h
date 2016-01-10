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
#ifndef SPRITESHEETGAMEOBJECT_H_
#define SPRITESHEETGAMEOBJECT_H_

#include <vector>
#include <string>
#include <map>


#include <SpriteComponent.h>
/*
#include "GameObject.h"
#include "PropertySet.h"
#include "SpriteSheet.h"
#include "SpriteAnimation.h"
#include "Map.h"
#include "Tileset.h"
#include "Text.h"
#include "Texture.h"
*/
namespace yam2d
{
	

class Tileset;
class Map;
class SpriteSheet;



/**
 * Class for SpriteSheetGameObject.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class SpriteSheetComponent : public SpriteComponent
{
public:
	// Default constructor.
	SpriteSheetComponent(GameObject* owner, SpriteSheet* spriteSheet, int idInSpriteSheet)
		: SpriteComponent(owner, spriteSheet->getTexture())
		, m_spriteSheet(spriteSheet)
		, m_id(idInSpriteSheet)
	{
	}

	virtual ~SpriteSheetComponent() {}

	void setIdInSpriteSheet(int idInSpriteSheet)
	{
		m_id = idInSpriteSheet;
	}
	
	SpriteSheet* getSpriteSheet() const { return m_spriteSheet.ptr(); }
	int getIdInSpriteSheet() const { return m_id; }

private:
	Ref<SpriteSheet>	m_spriteSheet;	// Sprite sheet
	unsigned			m_id;			// Id at sprite sheet

	SpriteSheetComponent();
	SpriteSheetComponent(const SpriteSheetComponent& o);
	SpriteSheetComponent& operator=(const SpriteSheetComponent& o);
};



}

#endif




