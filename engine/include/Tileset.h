#ifndef TILESET_H_
#define TILESET_H_

#include <string>
#include "PropertySet.h"
#include <Ref.h>
#include <SpriteSheet.h>

namespace yam2d
{


class SpriteSheet;

/**
 * Class for tileset.
 */
class Tileset : public Object
{
public:
	/**
	 * Constructs new Tileset-object according to parameters.
	 * @param name				Human readable name of this tileset.
	 * @param spriteSheet		Sprite sheet.
	 * @param tileOffsetX		Tile offset x to be used in rendering of sprites from this Tileset.
	 * @param tileOffsetY		Tile offset y to be used in rendering of sprites from this Tileset.
	 * @param properties		Properties of this tileset.
	 */
	Tileset(const std::string name, SpriteSheet* spriteSheet, float tileOffsetX, float tileOffsetY, const PropertySet& properties=PropertySet() );

	virtual ~Tileset() {}

	/** Returns SpriteSheet of this Tileset */
	SpriteSheet* getSpriteSheet() const { return m_spriteSheet.ptr(); }

	/** Returns offset x to be used in rendering of sprites from this Tileset. */
	float getTileOffsetX()  { return m_tileOffsetX; }

	/** Returns offset x to be used in rendering of sprites from this Tileset. */
	float getTileOffsetY() { return m_tileOffsetY; }

private:
	std::string			m_name;
	PropertySet			m_properties;
	float				m_tileOffsetX;
	float				m_tileOffsetY;

	Ref<SpriteSheet>	m_spriteSheet;

	// Hidden
	Tileset();
	Tileset(const Tileset&);
	Tileset& operator=(const Tileset&);
};


}


#endif

