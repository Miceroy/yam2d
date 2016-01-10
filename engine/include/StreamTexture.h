// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// http://code.google.com/p/yam2d/
//
// Copyright (c) 2015 Mikko Romppainen
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
#ifndef STREAM_TEXTURE_H_
#define STREAM_TEXTURE_H_

#include <Texture.h>
#include <string>

namespace yam2d
{

	/**
	 * Class for Streaming Texture, which can be used for example textures, which contents is needed to
	 * update ocasionally. Note: Current implementation does not use any specific buffering. It would be
	 * good idea to add some kind of (ring) buffering / streaming extension stuff so that OpenGL will have
	 * time to update texture data to GPU before drawing.
	 *
	 * @ingroup yam2d
	 * @author Mikko Romppainen (mikko@kajakbros.com)
	 */
	class StreamTexture : public yam2d::Texture
	{
	public:
		StreamTexture();
		virtual ~StreamTexture();
		virtual int getNativeId() const;

		void setData(unsigned char* data, int width, int height, int bpp);
	private:
	};

}

#endif 


