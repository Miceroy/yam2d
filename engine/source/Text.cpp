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

Text::Text(GameObject* owner, SpriteSheet* font)
	: Component(owner, Component::getDefaultProperties())
, m_font(font)
, m_sprite( new Sprite(0) )
, m_totalWidth(0)
, m_text("")
{
}


void Text::setText( const std::string& str )
{
	m_text = str;

	m_totalWidth=0;
	const char* c = m_text.c_str();

	while( *c != '\0' )
	{
		unsigned char clipIndex = *c;
		++c;
		m_totalWidth += m_font->getClip(clipIndex).clipSize.x;
	}
}


void Text::setText( const char* str )
{
	setText( std::string(str) );
}


void Text::getVertexData( std::vector<float>& verts, std::vector<float>& texCoords, std::vector<float>& colors ) const
{
	const char* c = m_text.c_str();
	float xOffset = 0;
	while( *c != '\0' )
	{
		unsigned char clipIndex = *c;
		++c;
		
		m_sprite.ptr()->setClip( float(m_font->getTexture()->getWidth()), float(m_font->getTexture()->getHeight()),m_font->getClip(clipIndex));
		size_t start = verts.size();
		m_sprite->getVertexData(verts,texCoords,colors);
		
		for( ; start<verts.size(); start+=3 )
		{
			verts[start+0] += (-m_totalWidth/2) + xOffset;
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
	

SpriteSheet* Text::getFont() const
{
	return m_font.ptr(); 
}
	

int Text::getWidth()
{
	return m_totalWidth;
}


}
