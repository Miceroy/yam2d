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

#include "SpriteBatch.h"
#include <es_util.h>
#include "Text.h"
#include <Texture.h>
#include <Sprite.h>
#include <SpriteSheet.h>

namespace yam2d
{

// anonymous namespace for internal functions
namespace
{
	int numTriangles = 0;
	int numDrawCalls = 0;
	int numSpritesBatched = 0;
	
	void swap(float& v1, float& v2)
	{
		float temp = v1;
		v1 = v2;
		v2 = temp;
	}

}


void SpriteBatch::resetStatsValues()
{
	numTriangles = 0;
	numDrawCalls = 0;
	numSpritesBatched = 0;
}


int SpriteBatch::getNumTriangles()
{
	return numTriangles;
}


int SpriteBatch::getNumDrawCalls()
{
	return numDrawCalls;
}


int SpriteBatch::getNumSpritesBatched()
{
	return numSpritesBatched;
}


SpriteBatch::SpriteBatch()
: m_positions()
, m_textureCoords()
, m_texture(0)
{
}


SpriteBatch::~SpriteBatch()
{
}


void SpriteBatch::setTexture(Texture* texture)
{
	m_texture = texture;
}


void SpriteBatch::addSprite(Sprite* sprite, const vec2& position, float rotation, const vec2& scale, const vec2& offset )
{
	assert( fabs(offset.x) < 0.01f && fabs(offset.y) < 0.01f ); // offset is not implemented.

	size_t start = m_positions.size();
	size_t startTex = m_textureCoords.size();

	sprite->getVertexData(m_positions,m_textureCoords,m_colors);

	// If negative scale (rotate triangles winding order)
	if( scale.x*scale.y < 0.0f )
	{
		for(size_t i=start; i<m_positions.size(); i += 2*3*3 )
		{
			swap( m_positions[i+ 0], m_positions[i+ 3] );
			swap( m_positions[i+ 1], m_positions[i+ 4] );
			swap( m_positions[i+ 2], m_positions[i+ 5] );

			swap( m_positions[i+ 9], m_positions[i+12] );
			swap( m_positions[i+10], m_positions[i+13] );
			swap( m_positions[i+11], m_positions[i+14] );
		}

		for(size_t i=startTex; i<m_textureCoords.size(); i += 2*3*2 )
		{
			swap( m_textureCoords[i+ 0], m_textureCoords[i+ 2] );
			swap( m_textureCoords[i+ 1], m_textureCoords[i+ 3] );

			swap( m_textureCoords[i+ 6], m_textureCoords[i+ 8] );
			swap( m_textureCoords[i+ 7], m_textureCoords[i+ 9] );
		}
	}

	for( ; start<m_positions.size(); start += 3 )
	{
		float x0 =  (m_positions[start+0]*scale.x);
		float y0 = (m_positions[start+1]*scale.y);
		
		float sinAngle = sinf(rotation);
		float cosAngle = cosf(rotation);

		// Rotation
		float x = x0*cosAngle - y0*sinAngle;
		float y = x0*sinAngle + y0*cosAngle;

		// Translation
		m_positions[start+0] = position.x + x;
		m_positions[start+1] = position.y + y;
		m_positions[start+2] = m_positions[start+2];
	}

	++numSpritesBatched;
}


void SpriteBatch::addText(Text* text, const vec2& position, float rotation, const vec2& scale,const vec2& offset )
{
	size_t start = m_positions.size();
	size_t startTex = m_textureCoords.size();

	text->getVertexData(m_positions,m_textureCoords,m_colors);

	// If negative scale (rotate triangles winding order)
	if( scale.x*scale.y < 0.0f )
	{
		for(size_t i=start; i<m_positions.size(); i += 2*3*3 )
		{
			swap( m_positions[i+ 0], m_positions[i+ 3] );
			swap( m_positions[i+ 1], m_positions[i+ 4] );
			swap( m_positions[i+ 2], m_positions[i+ 5] );

			swap( m_positions[i+ 9], m_positions[i+12] );
			swap( m_positions[i+10], m_positions[i+13] );
			swap( m_positions[i+11], m_positions[i+14] );
		}

		for(size_t i=startTex; i<m_textureCoords.size(); i += 2*3*2 )
		{
			swap( m_textureCoords[i+ 0], m_textureCoords[i+ 2] );
			swap( m_textureCoords[i+ 1], m_textureCoords[i+ 3] );

			swap( m_textureCoords[i+ 6], m_textureCoords[i+ 8] );
			swap( m_textureCoords[i+ 7], m_textureCoords[i+ 9] );
		}
	}

	for( ; start<m_positions.size(); start += 3 )
	{
		float x0 = offset.x+m_positions[start+0]*scale.x;
		float y0 = offset.y+m_positions[start+1]*scale.y;
		
		float sinAngle = sinf(rotation);
		float cosAngle = cosf(rotation);

		// Rotation
		float x = x0*cosAngle - y0*sinAngle;
		float y = x0*sinAngle + y0*cosAngle;

		// Translation
		m_positions[start+0] = position.x + x;
		m_positions[start+1] = position.y + y;
		m_positions[start+2] = m_positions[start+2];
	}

}


void SpriteBatch::clear()
{
	m_positions.clear();
	m_textureCoords.clear();
	m_colors.clear();
}


void SpriteBatch::render(float aspectRatio)
{
	if( m_positions.size() == 0 )
		return;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &m_positions[0]);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, &m_textureCoords[0]);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, &m_colors[0]);

	if( m_texture )
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_texture->getNativeId());
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(1,aspectRatio,1);
	numTriangles += m_positions.size()/3;
	++numDrawCalls;
	glDrawArrays(GL_TRIANGLES, 0, m_positions.size()/3);

	glPopMatrix();

	if( m_texture )
	{
		glDisable(GL_TEXTURE_2D);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}

Texture* SpriteBatch::getTexture() const
{
	return m_texture.ptr();
}




SpriteBatchGroup::SpriteBatchGroup()
{
}


SpriteBatchGroup::~SpriteBatchGroup()
{
}

	
void SpriteBatchGroup::addSprite(Texture* texture, Sprite* sprite, const vec2& position, float rotation, const vec2& scale, const vec2& offset )
{
	getBatch(texture)->addSprite(sprite,position,rotation,scale,offset);
}


void SpriteBatchGroup::addText(Texture* texture, Text* text, const vec2& position, float rotation, const vec2& scale,const vec2& offset )
{
	getBatch(texture)->addText(text,position,rotation,scale,offset);
}


void SpriteBatchGroup::clear()
{
	for( std::map<Texture*, Ref<SpriteBatch> >::iterator it = m_spriteBatches.begin(); it != m_spriteBatches.end(); ++it )
	{
		SpriteBatch* batch = it->second;
		batch->clear();
	}
}


void SpriteBatchGroup::render(float aspectRatio)
{
	for( std::map<Texture*, Ref<SpriteBatch> >::iterator it = m_spriteBatches.begin(); it != m_spriteBatches.end(); ++it )
	{
		SpriteBatch* batch = it->second;
		batch->render(aspectRatio);
	}
}


SpriteBatch* SpriteBatchGroup::getBatch(Texture* texture)
{
	SpriteBatch* batch = m_spriteBatches[texture];
	if( batch == 0 )
	{
		batch = new SpriteBatch();
		batch->setTexture(texture);
		m_spriteBatches[texture] = batch;
	}
	
	return batch;
}


}
