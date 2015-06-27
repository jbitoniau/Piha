#pragma once

#include "PihaImageFormat.h"
#include "PihaMemoryBuffer.h"

namespace Piha
{

/*
	Image
*/
class Image
{
public:
	Image();
	Image( const ImageFormat& imageFormat );
	Image( const Image& other );
	~Image();

	const ImageFormat&				getFormat() const		{ return mFormat; }
	
	MemoryBuffer&					getBuffer()				{ return *mBuffer; }
	const MemoryBuffer&				getBuffer() const		{ return *mBuffer; }

	// Create an image from a format and an existing MemoryBuffer. The provided buffer must be compatible with 
	// the image format (the method returns NULL if it's not), i.e it must be the right size. 
	// The created Image owns the MemoryBuffer. Deleting the Image will delete the MemoryBuffer.
	static Image* make( const ImageFormat& imageFormat, MemoryBuffer* memoryBuffer );

protected:
	Image( const ImageFormat& imageFormat, MemoryBuffer* memoryBuffer );

private:
	ImageFormat						mFormat;
	MemoryBuffer*					mBuffer;	
};

}