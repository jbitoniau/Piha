#include "PihaImageFormat.h"

#include <assert.h>
#include <sstream>

namespace Piha
{

unsigned int ImageFormat::mEncodingBitsPerPixel[EncodingCount] = 
{
	24,
	24,
	32,
	16
};

const char* ImageFormat::mEncodingNames[EncodingCount] = 
{
	"RGB24",
	"BGR24",
	"BGRX32",
	"YUYV"	
};
	
ImageFormat::ImageFormat()
	: mWidth(0), 
	  mHeight(0), 
	  mEncoding(RGB24)
{
}

ImageFormat::ImageFormat( unsigned int width, unsigned int height, Encoding encoding )
	: mWidth(width), 
	  mHeight(height), 
	  mEncoding(encoding)
{
}

unsigned int ImageFormat::getNumBitsPerPixel( Encoding encoding )
{
	if ( encoding>=EncodingCount )
		return 0;
	return mEncodingBitsPerPixel[encoding];
}

const char* ImageFormat::getEncodingName( Encoding encoding )
{
	if ( encoding>=EncodingCount )
		return "Unknown";
	return mEncodingNames[encoding];
}

unsigned int ImageFormat::getDataSizeInBytes() const
{
	unsigned int size = getHeight() * getNumBytesPerLine();
	return size;
}

bool ImageFormat::operator==( const ImageFormat& other ) const
{
	return	mWidth == other.mWidth && 
			mHeight == other.mHeight &&
			mEncoding == other.mEncoding;
}

bool ImageFormat::operator!=( const ImageFormat& other ) const
{
	return	!( *this==other );
}

std::string ImageFormat::toString() const
{
	std::stringstream stream;
	stream << getWidth() << "x" << getHeight() << " pixels, " << getEncodingName() << " encoding";
	return stream.str();
}

}
