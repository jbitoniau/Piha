#pragma once

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )	
#endif
#include <QFrame>
#include <QPainter>
#include <QKeyEvent>
#ifdef _MSC_VER
	#pragma warning( pop )
#endif

#include "PihaImage.h"
#include "PihaImageConverter.h"

namespace Piha
{

class QRGB888ImageMaker;

class QImageWidget: public QFrame
{ 
	Q_OBJECT

public:
	QImageWidget( QWidget* parent, Qt::WindowFlags flags=0 );
	virtual ~QImageWidget();

	void				setImage( const Piha::Image& image );
	
	void				scaleImage( bool scale )	{ mScaleImage = scale; }
	bool				isImageScaled() const		{ return mScaleImage; }

signals:
	void				mousePressed();

protected:
	virtual void		paintEvent( QPaintEvent* paintEvent );
	virtual void		mousePressEvent( QMouseEvent* event );
	virtual void		keyPressEvent ( QKeyEvent* event );

private:
	QRGB888ImageMaker*	mQImageMaker;
	bool				mScaleImage;
	bool				mVerticalFlip;
	bool				mHorizontalFlip;
};

/*
	Produces a QImage with the QT RGB888 format from a Piha::Image
	(performs the necessary conversion under the hood)	

	Note: with QT's RGB888 format, if you read the QImage buffer *byte after byte* 
	then the color components appear in this order: red, green, blue.
	This format is strictly equivalent to Piha::ImageFormat::RGB24.
*/
class QRGB888ImageMaker
{
public:
	QRGB888ImageMaker( int width, int height );
	~QRGB888ImageMaker();
	
	bool			update( const Image& image );
	const QImage&	getQImage() const { return *mQImage; }

private:
	QImage*			mQImage;
	ImageConverter*	mImageConverter;
};


}