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
#ifndef FILE_STREAM_H_
#define FILE_STREAM_H_
#include <Stream.h>

#include <stdio.h>

#if defined(ANDROID)
	struct AAsset;
#endif

namespace yam2d
{

/**
 * Stream implementation for regular file system stream.
 */
class FileStream : public Stream
{
public:
	enum FileOpenMode
	{
		READ_ONLY = 0,
		READ_WRITE = 1
	};

	/** Constructor */
	FileStream( const char* const fileName, FileOpenMode mode );
	
	/** Destructor */
	virtual ~FileStream( );

	/**
	 * Writes n bytes to the stream.
	 * @param p Data to write.
	 * @param n Number of bytes to write.
	 * @return Number of (uncompressed) bytes written.
	 */
	virtual void write( const void* p, int n );

	/**
	 * Reads n bytes from the stream.
	 * @param p [out] Receives read bytes
	 * @param n Number of bytes to read.
	 * @return Number of (uncompressed) bytes read.
	 */
	virtual int read( void* p, int n );

	/**
	 * Returns number of bytes, which can be read whitout blocking.
	 */
	virtual int	available() const;

	

private:
#if defined(_WIN32)
	typedef FILE* FileHandleType;
#elif defined(ANDROID)
	typedef AAsset* FileHandleType;
	long m_available;
#else
	Missing file handle type
#endif

	FileHandleType m_file;
	FileOpenMode m_mode;

	FileStream();
	FileStream( const FileStream& );
	FileStream& operator=( const FileStream& );
};

}


#endif

