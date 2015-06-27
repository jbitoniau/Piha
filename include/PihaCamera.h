#pragma once

#include "PihaComponent.h"

#include <vector>
#include "PihaImage.h"

namespace Piha
{

/*
	CaptureSettings
*/
class CaptureSettings
{
public:
	CaptureSettings();
	CaptureSettings( const ImageFormat& imageFormat, float frameRate );
	bool operator==( const CaptureSettings& other ) const;

	const ImageFormat&	getImageFormat() const	{ return mImageFormat; }
	float				getFrameRate() const	{ return mFrameRate; }
	
	std::string			toString() const;

private:
	ImageFormat			mImageFormat;
	float				mFrameRate;
};
typedef std::vector<CaptureSettings> CaptureSettingsList;

/*
	CapturedImage
*/
class CapturedImage
{
public:
	// Create a CapturedImage from an ImageFormat. An Image based on this format is automatically created
	CapturedImage( const ImageFormat& imageFormat );

	// Create a CapturedImage from an existing Image. The provided Image is owned by the CapturedImage
	CapturedImage( Image* image );

	~CapturedImage();

	const Image&	getImage() const									{ return *mImage; }
	unsigned int	getSequenceNumber() const							{ return mSequenceNumber; }
	float			getTimestampInSec()	const							{ return mTimestampInSec; }

	Image&			getImage()											{ return *mImage; }
	void			setSequenceNumber( unsigned int	sequenceNumber )	{ mSequenceNumber = sequenceNumber; }
	void			setTimestampInSec( float timestamp )				{ mTimestampInSec = timestamp; }

private:
	Image*			mImage;
	unsigned int	mSequenceNumber;
	float			mTimestampInSec;
};

/*
	Camera
*/
class Camera : public Component
{
public:
	Camera( const std::string& name );

	const CaptureSettingsList&	getSupportedCaptureSettings() const							{ return mSupportedCaptureSettings; }
	bool						isCapturing() const											{ return mIsCapturing; }
	bool						getStartedCaptureSettingsIndex( unsigned int& index ) const;	// JBM: this is a bit cumbersome!
	bool						startCapture( std::size_t captureSettingsIndex );	
	void						stopCapture();
	const CapturedImage*		getCapturedImage() const;
	
	virtual std::string			toString() const;

protected:
	virtual bool				internalStartCapture(  std::size_t /*captureSettingsIndex*/ )	{ return false; }
	virtual void				internalStopCapture() {}
	virtual const CapturedImage* internalGetCapturedImage() const { return NULL; }

	CaptureSettingsList			mSupportedCaptureSettings;		// JBM: internal virtual for that?

private:
	bool						mIsCapturing;
	std::size_t					mStartedCaptureSettingsIndex;
};

}