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
#ifndef SPRITESHEET_H_
#define SPRITESHEET_H_

#include <vector>
#include "Sprite.h"

namespace yam2d
{
	
class Texture;

bool isWhiteOrTransparentPixel(const unsigned char* const data, int bpp);
bool isBlackOrTransparentPixel(const unsigned char* const data, int bpp);
bool isYellowOrTransparentPixel(const unsigned char* const data, int bpp);
bool isRedOrTransparentPixel(const unsigned char* const data, int bpp);

typedef bool (*IsPixelFunc)(const unsigned char* const data, int bpp);


/**
 * Class for SpriteSheet.
 *
 * @ingroup yam2d
 * @author Mikko Romppainen (mikko@kajakbros.com) 
 */
class SpriteSheet : public Object
{
public:
	SpriteSheet(Texture* texture, std::vector<Sprite::PixelClip> clips);

	virtual ~SpriteSheet() {}


	Texture* getTexture() { return m_texture; }
	const Texture* getTexture() const { return m_texture; }
	const Sprite::PixelClip& getClip(int index) const { return m_clips[index]; }
private:
	Texture* m_texture;
	std::vector<Sprite::PixelClip> m_clips;
};

SpriteSheet* autoFindSpriteSheetFromTexture(Texture* texture, IsPixelFunc isPixel = isWhiteOrTransparentPixel);
SpriteSheet* autoFindFontFromTexture(Texture* texture, const char* const fontWidthBinFileName = 0);
SpriteSheet* generateSpriteSheet(Texture* texture, int tileWidth, int tileHeight, int margin, int spacing );


}


#endif // SPRITESHEET_H_

