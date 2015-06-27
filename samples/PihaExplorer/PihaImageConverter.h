#pragma once

#include "PihaImage.h"

namespace Piha
{

class ImageConverter
{
public:
	ImageConverter( const ImageFormat& outputImageFormat );
	virtual ~ImageConverter();

	bool			update( const Image& sourceImage );
	const Image&	getImage() const			{ return *mImage; }
	Image&			getImage()					{ return *mImage; }

	static bool		convertBGR24ImageToRGB24Image( const Image& sourceImage, Image& destImage );
	static bool		convertBGRX32ImageToRGB24Image( const Image& sourceImage, Image& destImage );
	static bool		convertBGRX32ImageToBGR24Image( const Image& sourceImage, Image& destImage );
	static bool		convertYUYVImageToRGB24Image( const Image& sourceImage, Image& destImage );
	static bool		convertYUYVImageToBGR24Image( const Image& sourceImage, Image& destImage );
	
	static bool		convertImage( const Image& source, Image& destinationImage );

private:
	Image*			mImage;
};

}