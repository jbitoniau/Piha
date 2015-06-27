#include "PihaImage.h"

#include <stdio.h>
#include <cstring>
#include <assert.h>

namespace Piha
{

// Construct an empty image with zero size. The image instance obtained can still  
// be filled with data later using the assignment operator (which can modify its format).
Image::Image()
	: mFormat(),
	  mBuffer(NULL)
{
	mBuffer = new MemoryBuffer();
}

// Construct a blank image of a specific format. The internal image data is allocated and zero-filled
Image::Image( const ImageFormat& imageFormat )
	: mFormat(imageFormat), 
	  mBuffer(NULL)
{
	mBuffer = new MemoryBuffer( imageFormat.getDataSizeInBytes() );
}

// Construct an image from another one. The source image data is copied during the process
Image::Image( const Image& other )
	: mFormat(other.getFormat()), 
	  mBuffer(NULL)
{
	mBuffer = new MemoryBuffer( other.getBuffer() );
}

Image::Image( const ImageFormat& imageFormat, MemoryBuffer* memoryBuffer )
	: mFormat(imageFormat), 
	  mBuffer(memoryBuffer)
{
	// The static make() method which is the only one allowed to call this ctro
	// guaranties that the following is respected
	assert( mBuffer );
	assert( mFormat.getDataSizeInBytes()==mBuffer->getSizeInBytes() );
}

Image* Image::make( const ImageFormat& imageFormat, MemoryBuffer* memoryBuffer )
{
	if ( !memoryBuffer )
		return NULL;
	if ( imageFormat.getDataSizeInBytes()!=memoryBuffer->getSizeInBytes() )
		return NULL;
	Image* image = new Image( imageFormat, memoryBuffer );
	return image;
}

Image::~Image()
{
	delete mBuffer;
	mBuffer = NULL;
}

}
