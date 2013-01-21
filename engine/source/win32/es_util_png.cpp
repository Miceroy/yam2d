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

#include <windows.h>
#include <lpng1513/png.h>
#include "es_util.h"
#include <stdlib.h>
#include "es_assert.h"
#include <config.h>

namespace yam2d
{


bool esLoadPNG( const char *fileName, unsigned char *buffer, int *width, int *height, int *bytesPerPixel )
{
	// open file and test for it being a png
	FILE *fp = fopen(fileName, "rb");
	if (!fp)
	{
		esLogEngineError("[%s] File %s could not be opened for reading", __FUNCTION__, fileName);
		return false;
	}
	
	png_byte header[8];
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
	{
		esLogEngineError("[%s] File %s is not recognized as a PNG file", __FUNCTION__, fileName);
		fclose(fp);
		return false;
	}

	// initialize stuff 
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		esLogEngineError("[%s] png_create_read_struct failed", __FUNCTION__);
		fclose(fp);
		return false;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		esLogEngineError("[%s] png_create_info_struct failed", __FUNCTION__);
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return false;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		esLogEngineError("[%s] Error during init_io", __FUNCTION__);
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return false;
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	*width = png_get_image_width(png_ptr, info_ptr);
	*height = png_get_image_height(png_ptr, info_ptr);
	png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	*bytesPerPixel = 0;
	
	if(color_type&PNG_COLOR_MASK_COLOR) 
	{
		*bytesPerPixel += 3;
	}

	if( color_type&PNG_COLOR_MASK_ALPHA )
	{
		*bytesPerPixel += 1;
	}

	if( *bytesPerPixel == 0 )
	{
		esLogEngineError("[%s] Color type %d not supported", __FUNCTION__, color_type );
        fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	}

	if( buffer != 0 )
	{
		//png_read_png(png_ptr, info_ptr, 0, NULL);

		int number_of_passes = png_set_interlace_handling(png_ptr);
		png_read_update_info(png_ptr, info_ptr);
	
		// read file 
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			esLogEngineError("[%s] Error during read_image", __FUNCTION__);
			fclose(fp);
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			return false;
		}

		png_bytep * row_pointers = new png_bytep[*height];
		unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
		assert( row_bytes == (*width)*(*bytesPerPixel) ); // Image is not 3 bytex per pixel nor 4 bytes per pixel
		for (int y=0; y<(*height); y++)
		{
			row_pointers[y] = new png_byte[row_bytes];
		}

		png_read_image(png_ptr, row_pointers);
	

		for (int i = 0; i <(*height); i++) 
		{
			memcpy(buffer+((*width)*(*bytesPerPixel) * i), row_pointers[i], row_bytes);
		}
		
		for (int y=0; y<(*height); y++)
		{
			delete [] row_pointers[y];
		}

		delete [] row_pointers;
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);

	return true;
}






}

