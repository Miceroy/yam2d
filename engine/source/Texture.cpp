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

#include "Texture.h"
#include "es_util.h"
#include <es_assert.h>
#include <config.h>
#include <stdint.h>

namespace yam2d
{
	namespace 
	{
		bool isNPot(int d)
		{
			return d==2 || d==4 || d==8 || d==16 || d==32 || d==64 || d==128 || d==256 || d==512 || d==1024 || d==2048;
 		}

		bool isNpotSquare(int w, int h)
		{
			return w==h && isNPot(w) && isNPot(h);

		}
	}

Texture::Texture(const std::string& fileName, bool allowNPOT)
: m_nativeIds(0)
, m_width(0)
, m_height(0)
, m_bpp(0)
, m_data(0)
, m_numNativeIds(1)
{
	m_nativeIds = (unsigned int*)new int[m_numNativeIds];	
	glGenTextures(m_numNativeIds, m_nativeIds);

	if( false == esLoadPNG(fileName.c_str(), 0, &m_width, &m_height, &m_bpp ) )
	{
		return;
	}
	
	if( allowNPOT==false && !isNpotSquare(m_width,m_height) )
	{
		esLogEngineDebug("Image %s, is not NPOT Square texture (w:%d, h:%d, bpp:%d)",
			fileName.c_str(), m_width, m_height, m_bpp );
	}

	m_data = new unsigned char[m_width*m_height*m_bpp];
	if( false == esLoadPNG(fileName.c_str(), m_data, &m_width, &m_height, &m_bpp ) )
	{
		return;
	}

	esLogEngineDebug("[%s] Image loaded w:%d, h:%d, bpp:%d", __FUNCTION__, m_width, m_height, m_bpp);
	
	setData(m_data,m_width,m_height,m_bpp,0);
}


Texture::Texture(unsigned int nativeId, int bytesPerPixel)
: m_nativeIds(0)
, m_width(0)
, m_height(0)
, m_bpp(bytesPerPixel)
, m_data(0)
, m_numNativeIds(1)
{
	m_nativeIds = (unsigned int*)new int[m_numNativeIds];
	glGenTextures(m_numNativeIds, m_nativeIds);
	m_nativeIds[0] = nativeId;
}


Texture::Texture(int numNativeIds)
: m_nativeIds(0)
, m_width(0)
, m_height(0)
, m_bpp(0)
, m_data(0)
, m_numNativeIds(numNativeIds)
{
	m_nativeIds = (unsigned int*)new int[m_numNativeIds];
	glGenTextures(m_numNativeIds, m_nativeIds);
}

Texture::~Texture()
{
	glDeleteTextures(m_numNativeIds, m_nativeIds);
	delete [] m_nativeIds;

	if( m_data != 0 )
	{
		delete [] m_data;
		m_data = 0;
	}
}


void Texture::setSize(int width, int height)
{
	m_width = width;
	m_height = height;
}


int Texture::getNativeId() const
{
	return getNativeId(0);
}

int Texture::getNativeId(int index) const
{
	assert( index < m_numNativeIds );
	return m_nativeIds[index];
}


void Texture::setData(const unsigned char* data, int width, int height, int bpp, int nativeIdIndex)
{
	m_width = width;
	m_height = height;
	m_bpp = bpp;

	if( data != m_data )
	{
		if( m_data != 0 )
		{
			delete [] m_data;
			m_data = 0;
		}
		m_data = new uint8_t[m_width*m_height*m_bpp];
		memcpy(m_data,data,m_width*m_height*m_bpp);
	}
	updateData(nativeIdIndex);
}

void Texture::updateData(int nativeIdIndex)
{
	GLenum fmt;
	if (m_bpp == 4)
	{
		fmt = GL_RGBA;
	}
	else if (m_bpp == 3)
	{
		fmt = GL_RGB;
	}
	else
	{
		yam2d::esLogMessage("[%s] Unsupported bytes per pixel: %d", __FUNCTION__, m_bpp);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_nativeIds[nativeIdIndex]);
	glTexImage2D(GL_TEXTURE_2D, 0, fmt, m_width, m_height, 0, fmt, GL_UNSIGNED_BYTE, m_data);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

int Texture::getWidth() const
{
	return m_width;
}


int Texture::getHeight() const
{
	return m_height;
}


void Texture::setTransparentColor(unsigned char r, unsigned char g, unsigned char b)
{
	assert( m_data != 0 );
	GLenum fmt;
	if( getBytesPerPixel() == 4 )
	{	
		fmt = GL_RGBA;
		for( int y=0; y<getHeight(); ++y )
		{
			for( int x=0; x<getWidth(); ++x )
			{
				unsigned char* data = getPixel(x,y);
				if( data[0] == r && data[1] == g && data[2] == b )
				{
					data[3] = 0;
				}
			}
		}
	}
	else if( getBytesPerPixel() == 3 )
	{		
		unsigned char* newData = new unsigned char[m_width*m_height*4];
		
		for( int y=0; y<getHeight(); ++y )
		{
			for( int x=0; x<getWidth(); ++x )
			{
				unsigned char* data = getPixel(x,y);
				unsigned char* d = &newData[(y*getWidth()+x)*4];
				d[0] = data[0];
				d[1] = data[1];
				d[2] = data[2];
				d[3] = 0xff;
				if( data[0] == r && data[1] == g && data[2] == b )
				{
					d[3] = 0x00;
				}
			}
		}

		m_bpp = 4;
		fmt = GL_RGBA;
		delete [] m_data;
		m_data = newData;
	}
	else
	{
		esLogEngineError("[%s] Unsupported bytes per pixel: %d", __FUNCTION__, m_bpp);
		return;
	}
	
	glBindTexture(GL_TEXTURE_2D, getNativeId());
	glTexImage2D(GL_TEXTURE_2D, 0, fmt, m_width, m_height, 0,  fmt, GL_UNSIGNED_BYTE, m_data );
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

}
