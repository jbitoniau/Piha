#include "PihaQComponentTreeItemEditorFactory.h"

#include <assert.h>
#include "PihaAccelerometer.h"	
#include "PihaGyroscope.h"	
#include "PihaMagnetometer.h"	
#include "PihaTemperatureSensor.h"	
#include "PihaBarometer.h"
#include "PihaGPSReceiver.h"
#include "PihaButton.h"	
#include "PihaAxis.h"	
#include "PihaVibrator.h"	
#include "PihaBattery.h"
#include "PihaCamera.h"
#include "PihaPWM.h"
//#include "PihaButtonIcon.xpm"

namespace Piha
{

//QPixmapCache QComponentTreeItemEditorFactory::mPixmapCache();

QIcon QComponentTreeItemEditorFactory::getComponentIcon( Component* /*component*/ )
{
	/*QPixmap* pixmap;
	if ( !mPixmapCache.find( QString::fromUtf8(component->getTypeName()), pixmap ) )
	{
		
	}

	if ( component->getType()==Component::kButton )
	{
		if ( mPixmapCache
		return QIcon( QPixmap(PihaButtonIcon_xpm) );
	}*/
	/*else if ( component->getType()==Component::kAxis )
	{
	}
	*/return QIcon();
}

QString QComponentTreeItemEditorFactory::getComponentState( Component* component )
{
	const int prec = 2;
	QString text;
	switch ( component->getType() )
	{
		case Component::kAccelerometer:
		{
			Accelerometer* accelerometer = static_cast<Accelerometer*>(component);
			const Vector3d& acc = accelerometer->getAccelerationInGs();
			text = QString("x:%1 y:%2 z:%3 (g's)").arg(acc.x(), 0, 'f', prec).arg(acc.y(), 0, 'f', prec).arg(acc.z(), 0, 'f', prec);
		}
		break;

		case Component::kGyroscope:
		{
			Gyroscope* gyroscope = static_cast<Gyroscope*>(component);
			const Vector3d& ang = gyroscope->getAngularRateInDegPerSec();
			text = QString("x:%1 y:%2 z:%3 (deg/sec)").arg(ang.x(), 0, 'f', prec).arg(ang.y(), 0, 'f', prec).arg(ang.z(), 0, 'f', prec);
		}
		break;

		case Component::kMagnetometer:
		{
			Magnetometer* magnetometer = static_cast<Magnetometer*>(component);
			const Vector3d& mag = magnetometer->getMagneticFieldInGauss();
			text = QString("x:%1 y:%2 z:%3 (gauss)").arg(mag.x(), 0, 'f', prec).arg(mag.y(), 0, 'f', prec).arg(mag.z(), 0, 'f', prec);
		}
		break;
		
		case Component::kTemperatureSensor:
		{
			TemperatureSensor* temperatureSensor = static_cast<TemperatureSensor*>(component);
			text = QString("Temperature:%1 °C").arg(temperatureSensor->getTemperatureInDegreeCelsius(), 0, 'f', prec);
		}
		break;
				
		case Component::kBarometer:
		{
			Barometer* barometer = static_cast<Barometer*>(component);
			text = QString("Pressure:%1 hPa").arg(barometer->getPressureInHPa(), 0, 'f', prec);
		}
		break;

		case Component::kGPSReceiver:
		{
			GPSReceiver* gpsReceiver = static_cast<GPSReceiver*>(component);
			GPSReceiver::Measure measure = gpsReceiver->getMeasure();
			double lat = measure.getLatitudeInDecDeg();
			double lon = measure.getLongitudeInDecDeg();
			double alt = measure.getAltitudeInM();
			text = QString("Lat:%1° Lon:%2° Alt:%3m").arg(lat, 0, 'f', 4).arg(lon, 0, 'f', 4).arg(alt, 0, 'f', 2);
		}
		break;

		case Component::kButton:
		{
			Button* button = static_cast<Button*>(component);
			if ( button->isPressed() )
				text = QString("Pressed");
			else
				text = QString("Not pressed");
		}
		break; 

		case Component::kAxis:
		{
			Axis* axis = static_cast<Axis*>(component);
			text = QString("Value:%1 (n:%2 r:%3)").arg( axis->getValue(), 0, 'f', prec).arg( axis->getNormalizedValue(), 0, 'f', prec).arg( axis->getRestValue(), 0, 'f', prec);
		}
		break;

		case Component::kVibrator:
		{
			Vibrator* vibrator = static_cast<Vibrator*>(component);
			text = QString("Value:%1 (m:%2)").arg( vibrator->getValue() ).arg( vibrator->getMaxValue() );
		}
		break;

		case Component::kBattery:
		{
			Battery* battery = static_cast<Battery*>(component);
			text = QString("Value:%1%").arg( battery->getBatteryLevel() * 100.f, 0, 'f', prec );
		}
		break; 

		case Component::kCamera:
		{
			Camera* camera = static_cast<Camera*>(component);
			if ( camera->isCapturing() )
			{
				const CapturedImage* image = camera->getCapturedImage();
				if ( image )
				{
					unsigned int imageNumber = image->getSequenceNumber();
					float timestamp = image->getTimestampInSec();
					if ( image )
						text = QString("Capturing (#%1, %2s)").arg(imageNumber).arg(timestamp, 0, 'f', 2);
				}
				else
				{
					text = "Capturing (no image)";
				}
			}
			else 
			{
				text = "Idle";
			}
		}
		break;

		case Component::kPWM:
		{
			PWM* pwm = static_cast<PWM*>(component);
			text = QString("Pulse width:%1 µs").arg( pwm->getPulseWidthInUs() );
		}
		break;

		default:
			text = "<Unknown>";
		break;
	}

	return text;
}

QComponentEditorWidget* QComponentTreeItemEditorFactory::createTreeItemEditor( Component* component, QWidget* parent )
{
	QComponentEditorWidget* editor = NULL;
	switch ( component->getType() )
	{
	case Component::kAccelerometer: editor = new QAccelerometerTreeItemEditor( static_cast<Accelerometer*>(component), parent );
		break;
	case Component::kGyroscope: editor = new QGyroscopeTreeItemEditor( static_cast<Gyroscope*>(component), parent );
		break;
	case Component::kMagnetometer: editor = new QMagnetometerTreeItemEditor( static_cast<Magnetometer*>(component), parent );
		break;
	case Component::kTemperatureSensor: editor = new QTemperatureSensorTreeItemEditor( static_cast<TemperatureSensor*>(component), parent );
		break;
	case Component::kBarometer: editor = new QBarometerTreeItemEditor( static_cast<Barometer*>(component), parent );
		break;
	case Component::kGPSReceiver: editor = new QGPSReceiverTreeItemEditor( static_cast<GPSReceiver*>(component), parent );
		break;
	case Component::kVibrator: editor = new QVibratorTreeItemEditor( static_cast<Vibrator*>(component), parent );
		break;
	case Component::kCamera: editor = new QCameraTreeItemEditor( static_cast<Camera*>(component), parent );
		break;
	case Component::kPWM: editor = new QPWMTreeItemEditor( static_cast<PWM*>(component), parent );
		break;
	default:
		editor = NULL;
		break;
	}
	return editor;
}

/*
	QAccelerometerTreeItemEditor
*/
QAccelerometerTreeItemEditor::QAccelerometerTreeItemEditor( Accelerometer* accelerometer, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mAccelerometer(accelerometer),
	  mButton(NULL)
{
	mButton = new QPushButton( this );
	bool ret = connect( mButton, SIGNAL( clicked(bool) ), this, SLOT( onButtonClicked(bool) ) );
	assert(ret);
	mButton->setText( mAccelerometer->isStarted()?"Stop":"Start" );
	mButton->setFixedSize( 40, 18 );
}

void QAccelerometerTreeItemEditor::onButtonClicked( bool /*checked*/ ) 
{
	if ( mAccelerometer->isStarted() )
		mAccelerometer->stop();
	else 
		mAccelerometer->start();
	mButton->setText( mAccelerometer->isStarted()?"Stop":"Start" );
}

/*
	QGyroscopeTreeItemEditor
*/
QGyroscopeTreeItemEditor::QGyroscopeTreeItemEditor( Gyroscope* gyroscope, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mGyroscope(gyroscope),
	  mButton(NULL)
{
	mButton = new QPushButton( this );
	bool ret = connect( mButton, SIGNAL( clicked(bool) ), this, SLOT( onButtonClicked(bool) ) );
	assert(ret);
	mButton->setText( mGyroscope->isStarted()?"Stop":"Start" );
	mButton->setFixedSize( 40, 18 );
}

void QGyroscopeTreeItemEditor::onButtonClicked( bool /*checked*/ ) 
{
	if ( mGyroscope->isStarted() )
		mGyroscope->stop();
	else 
		mGyroscope->start();
	mButton->setText( mGyroscope->isStarted()?"Stop":"Start" );
}

/*
	QMagnetometerTreeItemEditor
*/
QMagnetometerTreeItemEditor::QMagnetometerTreeItemEditor( Magnetometer* magnetometer, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mMagnetometer(magnetometer),
	  mButton(NULL)
{
	mButton = new QPushButton( this );
	bool ret = connect( mButton, SIGNAL( clicked(bool) ), this, SLOT( onButtonClicked(bool) ) );
	assert(ret);
	mButton->setText( mMagnetometer->isStarted()?"Stop":"Start" );
	mButton->setFixedSize( 40, 18 );
}

void QMagnetometerTreeItemEditor::onButtonClicked( bool /*checked*/ ) 
{
	if ( mMagnetometer->isStarted() )
		mMagnetometer->stop();
	else 
		mMagnetometer->start();
	mButton->setText( mMagnetometer->isStarted()?"Stop":"Start" );
}

/*
	QTemperatureSensorTreeItemEditor
*/
QTemperatureSensorTreeItemEditor::QTemperatureSensorTreeItemEditor( TemperatureSensor* temperatureSensor, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mTemperatureSensor(temperatureSensor),
	  mButton(NULL)
{
	mButton = new QPushButton( this );
	bool ret = connect( mButton, SIGNAL( clicked(bool) ), this, SLOT( onButtonClicked(bool) ) );
	assert(ret);
	mButton->setText( mTemperatureSensor->isStarted()?"Stop":"Start" );
	mButton->setFixedSize( 40, 18 );
}

void QTemperatureSensorTreeItemEditor::onButtonClicked( bool /*checked*/ ) 
{
	if ( mTemperatureSensor->isStarted() )
		mTemperatureSensor->stop();
	else 
		mTemperatureSensor->start();
	mButton->setText( mTemperatureSensor->isStarted()?"Stop":"Start" );
}

/*
	QBarometerTreeItemEditor
*/
QBarometerTreeItemEditor::QBarometerTreeItemEditor( Barometer* barometer, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mBarometer(barometer),
	  mButton(NULL)
{
	mButton = new QPushButton( this );
	bool ret = connect( mButton, SIGNAL( clicked(bool) ), this, SLOT( onButtonClicked(bool) ) );
	assert(ret);
	mButton->setText( mBarometer->isStarted()?"Stop":"Start" );
	mButton->setFixedSize( 40, 18 );
}

void QBarometerTreeItemEditor::onButtonClicked( bool /*checked*/ ) 
{
	if ( mBarometer->isStarted() )
		mBarometer->stop();
	else 
		mBarometer->start();
	mButton->setText( mBarometer->isStarted()?"Stop":"Start" );
}


/*
	QGPSReceiverTreeItemEditor
*/
QGPSReceiverTreeItemEditor::QGPSReceiverTreeItemEditor( GPSReceiver* gpsReceiver, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mGPSReceiver(gpsReceiver),
	  mButton(NULL)
{
	mButton = new QPushButton( this );
	bool ret = connect( mButton, SIGNAL( clicked(bool) ), this, SLOT( onButtonClicked(bool) ) );
	assert(ret);
	mButton->setText( mGPSReceiver->isStarted()?"Stop":"Start" );
	mButton->setFixedSize( 40, 18 );
}

void QGPSReceiverTreeItemEditor::onButtonClicked( bool /*checked*/ ) 
{
	if ( mGPSReceiver->isStarted() )
		mGPSReceiver->stop();
	else 
		mGPSReceiver->start();
	mButton->setText( mGPSReceiver->isStarted()?"Stop":"Start" );
}

/*
	QVibratorTreeItemEditor
*/
QVibratorTreeItemEditor::QVibratorTreeItemEditor( Vibrator* vibrator, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mVibrator(vibrator),
	  mSlider(NULL)
{
	mSlider = new QSlider( Qt::Horizontal, this );
	mSlider->setMinimum(0);
	mSlider->setMaximum( vibrator->getMaxValue() );
	mSlider->setSingleStep( vibrator->getMaxValue() / 50 );
	bool ret = connect( mSlider, SIGNAL( valueChanged (int) ), this, SLOT( onSliderValueChanged(int) ) );
	assert(ret);
	mSlider->setFixedSize( 100, 18 );
}

void QVibratorTreeItemEditor::updateContents()
{
	unsigned int vibratorValue = mVibrator->getValue();
	unsigned int v = static_cast<unsigned int>( mSlider->value() );
	if ( vibratorValue!=v )
		mSlider->setValue( vibratorValue );
}

void QVibratorTreeItemEditor::onSliderValueChanged( int value ) 
{
	unsigned int v = static_cast<unsigned int>(value);	
	mVibrator->setValue( v );
}

/*
	QCameraTreeItemEditor
*/
QCameraTreeItemEditor::QCameraTreeItemEditor( Camera* camera, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mCamera(camera),
	  mCaptureSettingsCombo(NULL),
	  mButton(NULL),
	  mImageWidget(NULL)
{
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	setLayout(layout);

	mButton = new QPushButton( this );
	layout->addWidget( mButton );
	bool ret = connect( mButton, SIGNAL( clicked(bool) ), this, SLOT( onButtonClicked(bool) ) );
	assert(ret);
	mButton->setText( mCamera->isCapturing()?"Stop":"Start" );
	mButton->setFixedSize( 40, 18 );

	mCaptureSettingsCombo = new QComboBox( this );
	layout->addWidget( mCaptureSettingsCombo );
	mCaptureSettingsCombo->setFixedSize( 200, 18 );
	const CaptureSettingsList& settingsList = camera->getSupportedCaptureSettings();
	for ( std::size_t i=0; i<settingsList.size(); ++i )
	{
		const CaptureSettings& settings = settingsList[i];
		unsigned int w = settings.getImageFormat().getWidth();
		unsigned int h = settings.getImageFormat().getHeight();
		const char* encoding = settings.getImageFormat().getEncodingName();
		float fps = settings.getFrameRate();
		QString text = QString("#%1 %2x%3 %4 %5fps").arg(i).arg(w).arg(h).arg(QString(encoding)).arg(fps, 0,'f',1);
		mCaptureSettingsCombo->addItem( text );
	}	
	
	mImageWidget = new QImageWidget( this );
	mImageWidget->setFrameShape( QFrame::Box );
	mImageWidget->setFixedSize( 24, 18 );
	mImageWidget->scaleImage(true);
	layout->addWidget( mImageWidget );

	ret = connect( mImageWidget, SIGNAL(mousePressed()), SLOT(onImageWidgetClicked()) );
	assert(ret);

	layout->addSpacerItem( new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum) );
}

void QCameraTreeItemEditor::onButtonClicked( bool /*checked*/ ) 
{
	if ( mCamera->isCapturing() )
	{
		mCamera->stopCapture();
		mCaptureSettingsCombo->setEnabled(true);
		mImageWidget->setImage( Image() );
		QImageWidget* preview = getImagePreviewWindow();
		if ( preview )
			preview->setImage( Image() );
	}
	else 
	{
		unsigned int index = mCaptureSettingsCombo->currentIndex();
		if ( mCamera->startCapture(index) )
			mCaptureSettingsCombo->setEnabled(false);
	}
	mButton->setText( mCamera->isCapturing()?"Stop":"Start" );
}

QImageWidget* QCameraTreeItemEditor::getImagePreviewWindow() const
{
	QImageWidget* widget = findChild<QImageWidget*>("ImagePreviewWindow");
	return widget;
}

QImageWidget* QCameraTreeItemEditor::createImagePreviewWindow()
{
	QImageWidget* widget = new QImageWidget(this, Qt::Window);
	QString cameraName = "Unnamed";
	if ( !mCamera->getName().empty() )
		QString( mCamera->getName().c_str() );
	QString text = QString("Device '%1' - Camera '%2'").arg( QString(mCamera->getParentDevice()->getName().c_str()) ).arg(cameraName);
	widget->setWindowTitle( text );
	widget->setObjectName("ImagePreviewWindow");
	widget->setAttribute(Qt::WA_DeleteOnClose);
	
	unsigned int settingsIndex = mCaptureSettingsCombo->currentIndex();
	const CaptureSettings& settings = mCamera->getSupportedCaptureSettings()[settingsIndex];
	widget->resize( settings.getImageFormat().getWidth(), settings.getImageFormat().getHeight() );
	return widget;
}

void QCameraTreeItemEditor::onImageWidgetClicked() 
{
	if ( getImagePreviewWindow() )
		return;
	QImageWidget* preview = createImagePreviewWindow();
	preview->show();
}

void QCameraTreeItemEditor::updateContents()
{
	const CapturedImage* capturedImage = mCamera->getCapturedImage();
	if ( capturedImage )
	{
		mImageWidget->setImage( capturedImage->getImage() );
		QImageWidget* preview = getImagePreviewWindow();
		if ( preview )
			preview->setImage( capturedImage->getImage() );
	}
}

/*
	QPWMTreeItemEditor
*/
QPWMTreeItemEditor::QPWMTreeItemEditor( PWM* pwm, QWidget* parent )
	: QComponentEditorWidget(parent),
	  mPWM(pwm),
	  mSlider(NULL)
{
	mSlider = new QSlider( Qt::Horizontal, this );
	mSlider->setMinimum(0);
	unsigned int range = PWM::getMaxPulseWidthInUs()-PWM::getMinPulseWidthInUs();
	mSlider->setMaximum( range );
	mSlider->setSingleStep( range / 50 );
	bool ret = connect( mSlider, SIGNAL( valueChanged (int) ), this, SLOT( onSliderValueChanged(int) ) );
	assert(ret);
	mSlider->setFixedSize( 100, 18 );
}

void QPWMTreeItemEditor::updateContents()
{
	unsigned int pulseWidth = mPWM->getPulseWidthInUs();
	unsigned int v = pulseWidth - PWM::getMinPulseWidthInUs();
	if ( v!=static_cast<unsigned int>(mSlider->value()) )
		mSlider->setValue( v );
}

void QPWMTreeItemEditor::onSliderValueChanged( int value ) 
{
	unsigned int v = static_cast<unsigned int>(value);
	unsigned int pulseWidth = PWM::getMinPulseWidthInUs() + v;
	bool ret = mPWM->setPulseWidthInUs( pulseWidth );
	
	// If changing the pulse width failed, we need to put the slider 
	// back to the width of the PWM component 
	if ( !ret )		
		updateContents();
}

}