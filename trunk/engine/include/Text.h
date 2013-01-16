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
#ifndef TEXT_H_
#define TEXT_H_

#include <vector>
#include <string>
#include <Object.h>

namespace yam2d
{
	
class SpriteSheet;
class Sprite;

/**
 * Class for Text.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class Text : public Object
{
public:
	Text(SpriteSheet* font);

	virtual ~Text() {}

	void setText( const std::string& str );
	void setText( const char* str );

	void getVertexData( std::vector<float>& verts, std::vector<float>& texCoords, std::vector<float>& colors ) const;

	/**
	 *  Sets depth value of this sprite.
	 */
	void setDepth( float depth );

	/**
	 * Sets color of this sprite.
	 */
	void setColor( float r, float g, float b, float a = 1.0f );
	void setOpacity( float a );
	SpriteSheet* getFont() const { return m_font; }
	int getWidth() { return m_totalWidth; }
private:
	SpriteSheet* m_font;
	Sprite* m_sprite;
	int m_totalWidth;
	std::string m_text;
};


}


#endif

