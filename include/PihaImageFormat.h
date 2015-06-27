#pragma once

#include <string>

namespace Piha
{

/*
	ImageFormat
*/
class ImageFormat
{
public:
	enum Encoding
	{
		RGB24, // 3 bytes per pixel. The byte sequence is: red, green, blue.
				// This encoding doesn't seem to be returned by Windows APIs (Media Foundation or DirectX),
				// so it's probably not a native encoding at the hardware level.
				// However it is a pretty common representation in some software libraries.
		
		BGR24,	// 3 bytes per pixel. The byte sequence is: blue, green, red.
				// Same as RGB24 but different order. This is very popular in Media Foundation 
	
		BGRX32,	// 4 bytes per pixel. The byte sequence is: blue, green, red and unused (set to 255)
	
		YUYV,	// 4 bytes per 2-pixel macroblock. The byte sequence: is Y0, U0, Y1, V0. 
				// Where Y0 and Y1 represent the luma component of each pixel 
				// and U0 and V0 represent the chroma component of both pixels.
				// Again quite popular in Media Foundation

		EncodingCount	
	};

	ImageFormat();
	ImageFormat( unsigned int width, unsigned int height, Encoding encoding );

	unsigned int			getWidth() const			{ return mWidth; }
	unsigned int			getHeight() const			{ return mHeight; }
	Encoding				getEncoding() const			{ return mEncoding; }
	const char*				getEncodingName() const		{ return getEncodingName( getEncoding() ); }
	static const char*		getEncodingName( Encoding encoding );
	
	unsigned int			getNumBitsPerPixel() const		{ return getNumBitsPerPixel( getEncoding() ); }
	static unsigned int		getNumBitsPerPixel( Encoding encoding );
	unsigned int			getNumBytesPerLine() const		{ return getNumBitsPerPixel()*getWidth()/8; }	// Note: rounded to the upper byte?
	unsigned int			getDataSizeInBytes() const;

	bool					operator==( const ImageFormat& other ) const;
	bool					operator!=( const ImageFormat& other ) const;

	std::string				toString() const;

private:
	
	static const char*		mEncodingNames[EncodingCount];
	static unsigned int		mEncodingBitsPerPixel[EncodingCount];
	
	unsigned int			mWidth;
	unsigned int			mHeight;
	Encoding				mEncoding;
};


}
