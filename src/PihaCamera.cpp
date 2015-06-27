#include "PihaCamera.h"

#include <assert.h>
#include <sstream>
#include <cmath>    // fabs

namespace Piha
{

/* 
	CaptureSettings
*/
CaptureSettings::CaptureSettings()
	: mImageFormat(),
	  mFrameRate(0)
{
}
CaptureSettings::CaptureSettings( const ImageFormat& imageFormat, float frameRate )
	: mImageFormat(imageFormat),
	  mFrameRate(frameRate)
{
}

bool CaptureSettings::operator==( const CaptureSettings& other ) const
{
	float sameFrameRate = std::abs( mFrameRate - other.mFrameRate ) < 0.00001f;
	return	mImageFormat==other.mImageFormat && sameFrameRate;
}

std::string CaptureSettings::toString() const
{
	std::stringstream stream;
	stream << mImageFormat.toString() << ", " << mFrameRate << " frames/s";
	return stream.str();
}

/* 
	CapturedImage
*/
CapturedImage::CapturedImage( const ImageFormat& imageFormat )
	: mImage(NULL),
	  mSequenceNumber(0),
	  mTimestampInSec(0.f)
{
	mImage = new Image( imageFormat );
}

CapturedImage::CapturedImage( Image* image )
	: mImage(image),
	  mSequenceNumber(0),
	  mTimestampInSec(0.f)
{
	assert( mImage );
}

CapturedImage::~CapturedImage()
{
	delete mImage;
	mImage = NULL;
}

/* 
	Camera
*/
Camera::Camera( const std::string& name )
	: Component(kCamera, name),
	  mSupportedCaptureSettings(),
	  mIsCapturing(false),
	  mStartedCaptureSettingsIndex(0)
{
}

bool Camera::getStartedCaptureSettingsIndex( unsigned int& index ) const	
{ 
	index = 0;
	if ( !isCapturing() )  
		return false; 
	index = mStartedCaptureSettingsIndex;
	return true;
}

bool Camera::startCapture( std::size_t captureSettingsIndex )
{
	if ( isCapturing() )
		return false;
	if ( captureSettingsIndex>=getSupportedCaptureSettings().size() )
		return false;

	if ( !internalStartCapture(captureSettingsIndex ) )
		return false;
	
	mIsCapturing = true;
	mStartedCaptureSettingsIndex = captureSettingsIndex;
	notifyComponentChanged();

	return  true;
}

void Camera::stopCapture()
{
	if ( !isCapturing() )
		return;
	internalStopCapture();
	mIsCapturing = false;
	mStartedCaptureSettingsIndex = 0;
	notifyComponentChanged();
}
	
const CapturedImage* Camera::getCapturedImage() const
{
	if ( !isCapturing() )
		return NULL;
	return internalGetCapturedImage();
}	

std::string	Camera::toString() const
{
	std::stringstream stream;
	stream << Component::toString();
	if ( isCapturing() )
		stream << "capturing frame #?";
	else 
		stream << "not capturing"; 
	return stream.str();
}

}

