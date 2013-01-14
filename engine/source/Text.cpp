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

#include "Text.h"
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Texture.h"

namespace yam2d
{

Text::Text(SpriteSheet* font)
: m_font(font)
, m_sprite( new Sprite() )
, m_text("")
{
}


void Text::setText( const char* str )
{
	m_text = str;
}


void Text::getVertexData( std::vector<float>& verts, std::vector<float>& texCoords, std::vector<float>& colors ) const
{
	int totalWidth=0;
	const char* str = m_text;

	while( *str != '\0' )
	{
		unsigned char clipIndex = *str;
		++str;
		totalWidth += m_font->getClip(clipIndex).clipSize.x;
	}

	str = m_text;
	float xOffset = 0;
	while( *str != '\0' )
	{
		unsigned char clipIndex = *str;
		++str;
		
		m_sprite->setClip( float(m_font->getTexture()->getWidth()), float(m_font->getTexture()->getHeight()),m_font->getClip(clipIndex));
		size_t start = verts.size();
		m_sprite->getVertexData(verts,texCoords,colors);
		
		for( ; start<verts.size(); start+=3 )
		{
			verts[start+0] += (-totalWidth/2) + xOffset;
		}

		xOffset += float(m_font->getClip(clipIndex).clipSize.x);
	}
}


void Text::setDepth( float depth )
{
	m_sprite->setDepth(depth);
}


void Text::setColor( float r, float g, float b, float a )
{
	m_sprite->setColor(r,g,b,a);
}


void Text::setOpacity( float a )
{
	m_sprite->setOpacity(a);
}

}
