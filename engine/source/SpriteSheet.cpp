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

#include "SpriteSheet.h"
#include "Texture.h"
#include "es_util.h"
#include <list>


namespace yam2d
{

// anonymous namespace for internal functions
namespace
{
	int findNextXPixel(const Texture* const tex, int w, int h, int startX, int startY, IsPixelFunc isPixel)
	{
		for( int x=startX; x<w; ++x )
		{
			for( int y=startY; y<h; ++y )
			{
				if( isPixel(tex->getPixel(x,y),tex->getBytesPerPixel()) )
				{
					return x;
				}
			}
		}

		return startX;
	}


	int findNextYPixel(const Texture* const tex, int w, int h, int startX, int startY, IsPixelFunc isPixel)
	{	
		for( int y=startY; y<h; ++y )
		{
			for( int x=startX; x<w; ++x )
			{
				if( isPixel(tex->getPixel(x,y),tex->getBytesPerPixel()) )
				{
					return y;
				}
			}
		}

		return startY;
	}


	int findNextXPixelReverse(const Texture* const tex, int w, int h, int startX, int startY, IsPixelFunc isPixel)
	{
		for( int x=w-1; x>=startX; --x )
		{
			for( int y=h-1; y>=startY; --y )
			{
				if( isPixel(tex->getPixel(x,y),tex->getBytesPerPixel()) )
				{
					return x;
				}
			}
		}

		return w;
	}


	int findNextYPixelReverse(const Texture* const tex, int w, int h, int startX, int startY, IsPixelFunc isPixel)
	{	
		for( int y=h-1; y>=startY; --y )
		{
			for( int x=w-1; x>=startX; --x )
			{
				if( isPixel(tex->getPixel(x,y),tex->getBytesPerPixel()) )
				{
					return y;
				}
			}
		}

		return h;
	}


	int findNextEmptyLineX(const Sprite::PixelClip& clip, const Texture* const tex, IsPixelFunc isPixel)
	{
		int w = clip.topLeft.x+clip.clipSize.x;
		int h = clip.topLeft.y+clip.clipSize.y;
		for( int x=clip.topLeft.x; x<w; ++x )
		{
			bool isEmptyLine = true;

			for( int y=clip.topLeft.y; y<h; ++y )
			{
				const unsigned char* p = tex->getPixel(x,y);
				if( isPixel(p,tex->getBytesPerPixel()) )
				{	
					isEmptyLine = false;
					break; 
				}
			}

			if( isEmptyLine )
			{
				return x;
			}
		}

		return w+1;
	}


	int findNextEmptyLineY(const Sprite::PixelClip& clip, const Texture* const tex, IsPixelFunc isPixel)
	{
		int w = clip.topLeft.x+clip.clipSize.x;
		int h = clip.topLeft.y+clip.clipSize.y;
	
		for( int y=clip.topLeft.y; y<h; ++y )
		{
			bool isEmptyLine = true;
				
			for( int x=clip.topLeft.x; x<w; ++x )
			{
				const unsigned char* p = tex->getPixel(x,y);
				if( isPixel(p,tex->getBytesPerPixel()) )
				{	
					isEmptyLine = false;
					break; 
				}
			}

			if( isEmptyLine )
			{
				return y;
			}
		}

		return h+1;
	}


	Sprite::PixelClip trimClipArea(const Sprite::PixelClip& clip, const Texture* tex, IsPixelFunc isPixel)
	{
		int clipStartY = findNextYPixel(tex, clip.topLeft.x+clip.clipSize.x, 
			clip.topLeft.y+clip.clipSize.y, clip.topLeft.x,	clip.topLeft.y, isPixel);

		int clipStartX = findNextXPixel(tex, clip.topLeft.x+clip.clipSize.x, 
			clip.topLeft.y+clip.clipSize.y,	clip.topLeft.x,	clip.topLeft.y, isPixel);

		int clipEndY = findNextYPixelReverse(tex, clip.topLeft.x+clip.clipSize.x,
			clip.topLeft.y+clip.clipSize.y, clip.topLeft.x,	clip.topLeft.y, isPixel);

		int clipEndX = findNextXPixelReverse(tex, clip.topLeft.x+clip.clipSize.x, 
			clip.topLeft.y+clip.clipSize.y, clip.topLeft.x,	clip.topLeft.y, isPixel);

		Sprite::PixelClip res;
		res.topLeft.x = clipStartX;
		res.topLeft.y = clipStartY;
		res.clipSize.x = clipEndX-res.topLeft.x+1;
		res.clipSize.y = clipEndY-res.topLeft.y+1;
		return res;
	}


	void trimClipAreas(std::vector<Sprite::PixelClip>& clips, const Texture* const texture, IsPixelFunc isPixel)
	{
		for( size_t i=0; i<clips.size(); ++i )
		{
			clips[i] =  trimClipArea(clips[i],texture,isPixel);
		}
	}


	std::vector<Sprite::PixelClip> findSpriteClipsFromTexture(Texture* texture, IsPixelFunc isPixel)
	{
		std::list<Sprite::PixelClip> res;

		int w = texture->getWidth();
		int h = texture->getHeight();

		Sprite::PixelClip first;
		first.topLeft.x = 0;
		first.topLeft.y = 0;
		first.clipSize.x = w;
		first.clipSize.y = h;
		res.push_back(first);
		bool hasChanged = true;
		for( std::list<Sprite::PixelClip>::iterator it = res.begin(); it != res.end(); )
		{
			hasChanged = false;
			// Trim clip area
			*it = trimClipArea(*it,texture,isPixel);
			Sprite::PixelClip clip = *it;

			// Find next empty y line
			int nextEmptyLineY = findNextEmptyLineY(clip,texture,isPixel);
			if( nextEmptyLineY <= (clip.topLeft.y+clip.clipSize.y) )
			{ 
				hasChanged = true;

				// Divide horizontally (cut according to y-axis)
				Sprite::PixelClip top = clip;
				top.clipSize.y = nextEmptyLineY-top.topLeft.y;

				Sprite::PixelClip bottom = clip;
				bottom.topLeft.y += top.clipSize.y+1;
				bottom.clipSize.y -= top.clipSize.y+1;

				*it = top;
				++it;
				res.insert(it,bottom);
				--it;
				--it;
				continue; // for
			}

			// Find next empty x line
			int nextEmptyLineX = findNextEmptyLineX(clip,texture,isPixel);
			if( nextEmptyLineX <= (clip.topLeft.x+clip.clipSize.x) )
			{ 
				hasChanged = true;
			
				// Divide vertically (cut according to x-axis)
				Sprite::PixelClip left = clip;
				left.clipSize.x = nextEmptyLineX-left.topLeft.x;

				Sprite::PixelClip right = clip;
				right.topLeft.x += left.clipSize.x+1;
				right.clipSize.x -= left.clipSize.x+1;
			
				*it = left;
				++it;
				res.insert(it,right);
				--it;
				--it;
				continue; // for
			}
			++it;		
		}

		std::vector<Sprite::PixelClip> ret;

		for( std::list<Sprite::PixelClip>::iterator it = res.begin(); it != res.end(); ++it )
		{
			Sprite::PixelClip clip = *it;
			ret.push_back(*it);
		}

		return ret;
	}

}



SpriteSheet* SpriteSheet::autoFindSpriteSheetFromTexture(Texture* texture, IsPixelFunc isPixel)
{
	std::vector<Sprite::PixelClip> ret = findSpriteClipsFromTexture(texture,isPixel);
		
	return new SpriteSheet(texture,ret);
}


SpriteSheet* SpriteSheet::autoFindFontFromTexture(Texture* texture, const char* const fontWidthBinFileName)
{	
	std::vector<Sprite::PixelClip> ret = findSpriteClipsFromTexture(texture,isRedOrTransparentPixel);
	trimClipAreas(ret, texture, isRedOrTransparentPixel);
	texture->setTransparentColor(0,0,0);
	
	if( fontWidthBinFileName != 0 && strlen(fontWidthBinFileName) > 0 )
	{
		texture->setTransparentColor(0xff,0,0);

		std::vector<unsigned short> widths = readFile(fontWidthBinFileName);
		for( size_t i=0; i<widths.size(); ++i )
		{
			unsigned short w = widths[i];
			if( i < ret.size() )
			{
				int center = ret[i].topLeft.x + (ret[i].clipSize.x/2);
				ret[i].topLeft.x = center - (w/2);
				ret[i].clipSize.x = w;
			}
		}
	}
	
	return new SpriteSheet(texture,ret);
}

SpriteSheet* SpriteSheet::generateSpriteSheet(Texture* texture, int width, int height, int tileWidth, int tileHeight, int marginX, int marginY, int spacingX, int spacingY)
{
	std::vector<Sprite::PixelClip> ret;
	int w = width;//texture->getWidth();
	int h = height;//texture->getHeight();

	for( int y=marginY; (y+tileHeight)<=h; y += tileHeight+spacingY )
	{
		for( int x=marginX; (x+tileWidth)<=w; x += tileWidth+spacingX )
		{
			Sprite::PixelClip c;
			c.topLeft.x = x;
			c.topLeft.y = y;
			c.clipSize.x = tileWidth;
			c.clipSize.y = tileHeight;
			ret.push_back(c);
		}
	}

	return new SpriteSheet(texture,ret);
}

SpriteSheet* SpriteSheet::generateSpriteSheet(Texture* texture, int tileWidth, int tileHeight, int marginX, int marginY, int spacingX, int spacingY)
{
	std::vector<Sprite::PixelClip> ret;
	int w = texture->getWidth();
	int h = texture->getHeight();

	for( int y=marginY; (y+tileHeight)<=h; y += tileHeight+spacingY )
	{
		for( int x=marginX; (x+tileWidth)<=w; x += tileWidth+spacingX )
		{
			Sprite::PixelClip c;
			c.topLeft.x = x;
			c.topLeft.y = y;
			c.clipSize.x = tileWidth;
			c.clipSize.y = tileHeight;
			ret.push_back(c);
		}
	}


	return new SpriteSheet(texture,ret);
}


bool isWhiteOrTransparentPixel(const unsigned char* const data, int bpp)
{
	if( bpp == 4 && data[3] <= 16 )
	{
		return false;
	}
	
	unsigned char TOL = 0xff;
	bool res = (data[0] == TOL) && (data[1] == TOL) && (data[2] == TOL);
	return !res;
}


bool isBlackOrTransparentPixel(const unsigned char* const data, int bpp)
{
	if( bpp == 4 && data[3] == 0x00 )
	{
		return false;
	}
	
	unsigned char TOL = 0x00;
	bool res = (data[0] == TOL) && (data[1] == TOL) && (data[2] == TOL);
	return !res;
}


bool isYellowOrTransparentPixel(const unsigned char* const data, int bpp)
{
	if( bpp == 4 && data[3] == 0x00 )
	{
		return false;
	}
	
	unsigned char TOL = 0xff;
	bool res = (data[0] == TOL) && (data[1] == TOL) && (data[2] == 0x00);
	return !res;
}


bool isRedOrTransparentPixel(const unsigned char* const data, int bpp)
{
	if( bpp == 4 && data[3] == 0x00 )
	{
		return false;
	}
	
	unsigned char TOL = 0xff;
	bool res = (data[0] == TOL) && (data[1] == 0x00) && (data[2] == 0x00);
	return !res;
}


SpriteSheet::SpriteSheet(Texture* texture, std::vector<Sprite::PixelClip> clips)
: m_texture(texture)
, m_clips(clips)
{
	assert(getClipCount() > 0); // No clips!!
}


SpriteSheet::~SpriteSheet() 
{
}


Texture* SpriteSheet::getTexture()
{
	return m_texture;
}


const Texture* SpriteSheet::getTexture() const 
{
	return m_texture.ptr(); 
}


const Sprite::PixelClip& SpriteSheet::getClip(int index) const
{ 
	return m_clips[index]; 
}

int SpriteSheet::getClipCount() const
{
	return int(m_clips.size());
}

}

