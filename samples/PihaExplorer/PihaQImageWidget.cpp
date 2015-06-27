#include "PihaQImageWidget.h"



namespace Piha
{

/*
	QImageWidget
*/
QImageWidget::QImageWidget( QWidget* parent, Qt::WindowFlags flags )
	: QFrame(parent, flags),
	  mQImageMaker(NULL),
	  mScaleImage(false),
	  mVerticalFlip(false),
	  mHorizontalFlip(false)
{
}

QImageWidget::~QImageWidget()
{
	delete mQImageMaker;
	mQImageMaker = NULL;
}

void QImageWidget::paintEvent( QPaintEvent* /*paintEvent*/ )
{
	QPainter painter(this);
	
	QColor backgroundColor( 180, 180, 180 );
	painter.setBrush(backgroundColor);
	painter.drawRect( 0, 0, width()-1, height()-1 );

	if ( mQImageMaker )
	{
		const QImage* image = &mQImageMaker->getQImage();
		QImage image2;
		if ( mVerticalFlip || mHorizontalFlip )
		{
			image2 = image->mirrored( mHorizontalFlip, mVerticalFlip );
			image = &image2;
		}
		if ( mScaleImage )
			painter.drawImage( QRect(0, 0, width(), height()), *image );
		else
			painter.drawImage( 0, 0, *image );
	}

	painter.setPen(QColor( 255, 255, 255 ));
	painter.setFont(QFont("Arial", 10));
	QString t;
	if ( mVerticalFlip )
		t = "V";
	if ( mHorizontalFlip )
		t = t + "H";
	painter.drawText( 5, 15, t );
}

void QImageWidget::mousePressEvent( QMouseEvent* /*event*/ )
{
	emit mousePressed();
}

void QImageWidget::keyPressEvent ( QKeyEvent* event )
{
	if ( event->key()==Qt::Key_V )
		mVerticalFlip = !mVerticalFlip;
	else if ( event->key()==Qt::Key_H )
		mHorizontalFlip = !mHorizontalFlip;
	else if ( event->key()==Qt::Key_S )
		scaleImage(!isImageScaled());
}
	
void QImageWidget::setImage( const Piha::Image& image )
{
	unsigned int width = image.getFormat().getWidth();
	unsigned int height = image.getFormat().getHeight();
	int qwidth = 0;
	int qheight = 0;
	if ( mQImageMaker )
	{
		qwidth = mQImageMaker->getQImage().width();
		qheight = mQImageMaker->getQImage().height();
	}

	if ( !mQImageMaker || qwidth!=static_cast<int>(width) || qheight!=static_cast<int>(height) )
	{
		delete mQImageMaker;
		mQImageMaker = new QRGB888ImageMaker( width, height );
	}
	mQImageMaker->update( image );

	update();
}

/*
	QRGB888ImageMaker
*/
QRGB888ImageMaker::QRGB888ImageMaker( int width, int height )
	: mQImage(NULL),
	  mImageConverter(NULL)
{
	ImageFormat rgbFormat( width, height, ImageFormat::RGB24 );
	mImageConverter = new ImageConverter( rgbFormat );
	
	// Get a grip onto the data of the image that serves as output of the ImageConverter
	uchar* data = reinterpret_cast<uchar*>( mImageConverter->getImage().getBuffer().getBytes() );

	// Create a QImage pointing *directly* onto this data
	mQImage = new QImage( data, width, height, QImage::Format_RGB888 );
}

QRGB888ImageMaker::~QRGB888ImageMaker()
{
	delete mQImage;
	mQImage = NULL;

	delete mImageConverter;
	mImageConverter = NULL;
}
	
bool QRGB888ImageMaker::update( const Image& image )
{
	// Here we just have to update the ImageConverter
	// It internally updates the Mefool::Image it contains
	// And this, without having to do anything, updates the QImage because
	// it shares its data with the the Mefool::Image we just updated
	return mImageConverter->update( image );
}

}

