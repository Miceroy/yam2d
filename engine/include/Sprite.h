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
#ifndef SPRITE_H_
#define SPRITE_H_

#include <vector>
#include <vec2.h>
#include <GameObject.h>

namespace yam2d
{

/**
 * Class for Sprite.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class Sprite : public Component
{
public:
	struct PixelClip
	{
		PixelClip()
			:topLeft(0)
			,clipSize(0)
		{
		}
		
		vec2int topLeft;
		vec2int clipSize;
	};

	Sprite(Entity* owner);

	virtual ~Sprite();

	virtual void construct() {}

	virtual void destroy() {}

	/**
	 *  Sets scale factor of this sprite.
	 */
	void setScale( const vec2& scale );

	/**
	 *  Sets depth value of this sprite.
	 */
	void setDepth( float depth );

	/**
	 * Sets color of this sprite.
	 */
	void setColor( float r, float g, float b, float a = 1.0f );

	void setOpacity(float a);
	/**
	 * Sets clip area of this sprite in texture coordinates.
	 */
	void setClip(const vec2& start, const vec2& size);

	const vec2& getClipSize() { return m_cropSize; }

	const vec2& getScale() const;

	void getVertexData( std::vector<float>& verts, std::vector<float>& texCoords, std::vector<float>& colors ) const;

	/**
	 * Sets clip area of this sprite in pixel coordinates.
	 */
	void setClip( float textureWidth, float textureHeight, PixelClip clip );
	
	GameObject* getGameObject() { return (GameObject*)getOwner(); }
	const GameObject* getGameObject() const { return (const GameObject*)getOwner(); }

private:
	float 		m_color[4];
	vec2		m_scale;
	vec2		m_cropStart;
	vec2		m_cropSize;
	float 		m_depth;
};



}


#endif

