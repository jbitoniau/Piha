#pragma once

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )	
#endif
#include <QPushButton>
#include <QSlider>
#include <QBoxLayout>
#include <QWidget>
#include <QPixmapCache>
#include <QComboBox>
#include <QLabel>
#ifdef _MSC_VER
	#pragma warning( pop )
#endif

#include "PihaQImageWidget.h"
#include "PihaDevice.h"

namespace Piha
{

class Component;
class Accelerometer;
class Gyroscope;
class Magnetometer;
class TemperatureSensor;
class Barometer;
class GPSReceiver;
class Vibrator;
class Camera;
class PWM;

class QComponentEditorWidget : public QWidget
{
	Q_OBJECT
public:
	QComponentEditorWidget( QWidget* parent ) : QWidget(parent) {}
	virtual void	updateContents() {}
};

class QComponentTreeItemEditorFactory
{
public:
	static QIcon					getComponentIcon( Component* component ); 
	static QString					getComponentState( Component* component );
	static QComponentEditorWidget*	createTreeItemEditor( Component* component, QWidget* parent );

private:
	//static QPixmapCache mPixmapCache;
};

/*
	QAccelerometerTreeItemEditor
*/
class QAccelerometerTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QAccelerometerTreeItemEditor( Accelerometer* accelerometer, QWidget* parent );

protected slots:
	void			onButtonClicked( bool checked );

private:
	Accelerometer*	mAccelerometer;
	QPushButton*	mButton;
};

/*
	QGyroscopeTreeItemEditor
*/
class QGyroscopeTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QGyroscopeTreeItemEditor( Gyroscope* gyroscope, QWidget* parent );

protected slots:
	void			onButtonClicked( bool checked );

private:
	Gyroscope*		mGyroscope;
	QPushButton*	mButton;
};

/*
	QMagnetometerTreeItemEditor
*/
class QMagnetometerTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QMagnetometerTreeItemEditor( Magnetometer* magnetometer, QWidget* parent );

protected slots:
	void			onButtonClicked( bool checked );

private:
	Magnetometer*	mMagnetometer;
	QPushButton*	mButton;
};

/*
	QTemperatureSensorTreeItemEditor
*/
class QTemperatureSensorTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QTemperatureSensorTreeItemEditor( TemperatureSensor* temperatureSensor, QWidget* parent );

protected slots:
	void			onButtonClicked( bool checked );

private:
	TemperatureSensor*	mTemperatureSensor;
	QPushButton*	mButton;
};

/*
	QBarometerTreeItemEditor
*/
class QBarometerTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QBarometerTreeItemEditor( Barometer* barometer, QWidget* parent );

protected slots:
	void			onButtonClicked( bool checked );

private:
	Barometer*		mBarometer;
	QPushButton*	mButton;
};

/*
	QGPSReceiverTreeItemEditor
*/
class QGPSReceiverTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QGPSReceiverTreeItemEditor( GPSReceiver* gpsReceiver, QWidget* parent );

protected slots:
	void			onButtonClicked( bool checked );

private:
	GPSReceiver*	mGPSReceiver;
	QPushButton*	mButton;
};

/*
	QVibratorTreeItemEditor
*/
class QVibratorTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QVibratorTreeItemEditor( Vibrator* vibrator, QWidget* parent );
	virtual void	updateContents();

protected slots:
	void			onSliderValueChanged( int value );

private:
	Vibrator*		mVibrator;
	QSlider*		mSlider;
};

/*
	QCameraTreeItemEditor
*/
class QCameraTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QCameraTreeItemEditor( Camera* camera, QWidget* parent );
	virtual void	updateContents();

	QImageWidget*	getImagePreviewWindow() const;
	QImageWidget*	createImagePreviewWindow();
	
protected slots:
	void			onButtonClicked( bool checked );
	void			onImageWidgetClicked();

private:
	Camera*			mCamera;
	QComboBox*		mCaptureSettingsCombo;
	QPushButton*	mButton;
	QImageWidget*	mImageWidget;
};

/*
	QPWMTreeItemEditor
*/
class QPWMTreeItemEditor : public QComponentEditorWidget
{
	Q_OBJECT

public:
	QPWMTreeItemEditor( PWM* pwm, QWidget* parent );
	virtual void	updateContents();

protected slots:
	void			onSliderValueChanged( int value );

private:
	PWM*			mPWM;
	QSlider*		mSlider;
};

}