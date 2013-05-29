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
#include <FileStream.h>
#include <es_assert.h>
#include <config.h>

#if defined(ANDROID)
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

extern struct android_app* g_androidState;

#endif

namespace yam2d
{

#if defined(_WIN32)
FileStream::FileStream( const char* const fileName, FileOpenMode mode )
: Stream()
, m_mode(mode)
{
	if( m_mode == READ_ONLY )
	{
		m_file = fopen(fileName, "rb");
	}
	else if( m_mode == READ_WRITE )
	{
		m_file = fopen(fileName, "wb");
	}
	else
	{
		assert( m_mode == READ_ONLY || m_mode == READ_WRITE );
	}

	if( !m_file )
	{
		esLogEngineError("[%s] File %s could not be opened", __FUNCTION__, fileName);
		assert( m_file != 0 );
	}
}
	
FileStream::~FileStream( )
{
	if( m_file )
	{
		fclose(m_file);
	}
}

void FileStream::write( const void* p, int n )
{
	assert( m_mode == READ_WRITE );
	size_t s = fwrite(p, 1, n, m_file);
	assert( s == size_t(n) );
}

int FileStream::read( void* p, int n )
{
	assert( m_mode == READ_ONLY || m_mode == READ_WRITE );
	assert(available() >= n );
	size_t s = fread(p, 1, n, m_file);
	assert( s == size_t(n) );
	return n;
}

int	FileStream::available() const
{
	long current = ftell(m_file);
	assert(current >= 0);
	fseek(m_file, 0, SEEK_END);
	long end = ftell(m_file);
	assert(end >= 0);
	fseek(m_file, current, SEEK_SET);
	int res = end-current;
	assert(res >= 0);
	return res;
}
#endif

#if defined(ANDROID)
FileStream::FileStream( const char* const fileName, FileOpenMode mode )
: Stream()
, m_mode(mode)
{
	if( m_mode == READ_ONLY )
	{
		const int BUFFER_SIZE=255;
		char buffer[BUFFER_SIZE];
		AAssetManager* assetManager = g_androidState->activity->assetManager;
		m_file = AAssetManager_open(assetManager, fileName, AASSET_MODE_BUFFER);
	}
	else 
	{
		assert( m_mode == READ_ONLY );
	}

	if( !m_file )
	{
		esLogEngineError("[%s] File %s could not be opened", __FUNCTION__, fileName);
		assert( m_file != 0 );
	}

	m_available = AAsset_getLength(m_file);
}
	
FileStream::~FileStream( )
{
	if( m_file )
	{
		AAsset_close(m_file);
	}
}

void FileStream::write( const void* p, int n )
{
	assert( m_mode == READ_WRITE );
	(void)p;
	(void)n;
}

int FileStream::read( void* p, int n )
{
	assert( m_mode == READ_ONLY );
	assert(available() >= n );

	long s = AAsset_read (m_file,p,n);
	assert( s == n );
	m_available -= n;
	return n;
}

int	FileStream::available() const
{
	return m_available;
}
#endif
}




