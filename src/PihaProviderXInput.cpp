#include "PihaProviderXInput.h"

#include <assert.h>
#include <sstream>
#include "PihaButton.h"
#include "PihaAxis.h"
#include "PihaVibrator.h"
#include "PihaBattery.h"

/*	Notes:
	For now, we only support the battery of the Controller, not the one of the optional headset connected to it. 
	The headset would need to appear as a child Device of this XBox 360 game controller
*/	
namespace Piha
{

/*
	RXIVibrator 
*/
class RXIVibrator : public Vibrator
{
public:
	RXIVibrator( const std::string& name, unsigned int maxValue, RXI::Controller::VibrationMotorID motorID );
	
protected:
	virtual void internalSetValue( unsigned int value );

private:
	RXI::Controller::VibrationMotorID mMotorID;
};

/*
	RXIDevice 
	A Piha device that wraps around a RXI::Controller
*/
class RXIDevice : public Device
{
public:
	RXIDevice( const std::string& identifier, RXI::Controller* rxiController );
	
	virtual void			update(); 

	RXI::Controller*		getRXIController() const { return mRXIController; }

private:
	RXI::Controller*		mRXIController;
	std::vector<Button*>	mButtons;	
	Axis*					mLeftStickHorizontalAxis;
	Axis*					mLeftStickVerticalAxis;
	Axis*					mRightStickHorizontalAxis;
	Axis*					mRightStickVerticalAxis;
	Axis*					mLeftTriggerAxis;
	Axis*					mRightTriggerAxis;
	RXIVibrator*			mLeftVibrator;
	RXIVibrator*			mRightVibrator;
	Battery*				mBattery;
};

/*
	RXIVibrator
*/
RXIVibrator::RXIVibrator( const std::string& name, unsigned int maxValue, RXI::Controller::VibrationMotorID motorID )
	:	Vibrator(name, maxValue),
		mMotorID(motorID)
{
}
	
void RXIVibrator::internalSetValue( unsigned int value )
{
	RXIDevice* rxiDevice = static_cast<RXIDevice*>( getParentDevice() );
	rxiDevice->getRXIController()->setVibrationMotorSpeed( mMotorID, static_cast<WORD>(value) );
}

/*
	RXIDevice
*/
RXIDevice::RXIDevice( const std::string& identifier, RXI::Controller* rxiController )
	:	Device( identifier, rxiController->getSubTypeName() ),
		mRXIController( rxiController ),
		mButtons(),
		mLeftStickHorizontalAxis(NULL),
		mLeftStickVerticalAxis(NULL),
		mRightStickHorizontalAxis(NULL),
		mRightStickVerticalAxis(NULL),
		mLeftTriggerAxis(NULL),
		mRightTriggerAxis(NULL),
		mLeftVibrator(NULL),
		mRightVibrator(NULL),
		mBattery(NULL)
{
	mButtons.resize( RXI::Controller::Button_Count, NULL );
	for ( unsigned int i=0; i<RXI::Controller::Button_Count; ++i )
	{
		RXI::Controller::ButtonID buttonID = static_cast<RXI::Controller::ButtonID>(i);
		if ( rxiController->hasButton(buttonID) )
		{
			Button* button = new Button( rxiController->getButtonName(buttonID) );
			mButtons[i] = button;
			addComponent( button );
		}
	}

	if ( rxiController->hasThumbstick( RXI::Controller::Thumbstick_Left ) )
	{
		std::string stickName = std::string(RXI::Controller::getThumbstickName(RXI::Controller::Thumbstick_Left));
		mLeftStickHorizontalAxis = new Axis( stickName + " (Horizontal)", 0.5f );
		addComponent( mLeftStickHorizontalAxis );
		mLeftStickVerticalAxis = new Axis( stickName + " (Vertical)", 0.5f );
		addComponent( mLeftStickVerticalAxis );
	}
	if ( rxiController->hasThumbstick( RXI::Controller::Thumbstick_Right ) )
	{
		std::string stickName = std::string(RXI::Controller::getThumbstickName(RXI::Controller::Thumbstick_Right));
		mRightStickHorizontalAxis = new Axis( stickName + " (Horizontal)", 0.5f );
		addComponent( mRightStickHorizontalAxis );
		mRightStickVerticalAxis = new Axis( stickName + " (Vertical)", 0.5f );
		addComponent( mRightStickVerticalAxis );
	}
	
	if ( rxiController->hasTrigger( RXI::Controller::Trigger_Left) )
	{
		mLeftTriggerAxis = new Axis( RXI::Controller::getTriggerName(RXI::Controller::Trigger_Left), 0.f );
		addComponent( mLeftTriggerAxis );
	}
	if ( rxiController->hasTrigger( RXI::Controller::Trigger_Right) )
	{
		mRightTriggerAxis = new Axis( RXI::Controller::getTriggerName(RXI::Controller::Trigger_Right), 0.f );
		addComponent( mRightTriggerAxis );
	}

	if ( rxiController->hasVibrationMotor( RXI::Controller::VibrationMotor_Left) )
	{
		mLeftVibrator = new RXIVibrator( RXI::Controller::getVibrationMotorName(RXI::Controller::VibrationMotor_Left), 65535, RXI::Controller::VibrationMotor_Left );
		addComponent( mLeftVibrator );
	}
	if ( rxiController->hasVibrationMotor( RXI::Controller::VibrationMotor_Right) )
	{
		mRightVibrator = new RXIVibrator( RXI::Controller::getVibrationMotorName(RXI::Controller::VibrationMotor_Right), 65535, RXI::Controller::VibrationMotor_Right );
		addComponent( mRightVibrator );
	}

	// Note: only Controller battery is exposed as a Piha battery for now (the headset's is ignored)
	if ( rxiController->hasBattery( RXI::Controller::Battery_Controller ) )
	{
		mBattery = new Battery("");
		addComponent( mBattery );
	}
}
	
void RXIDevice::update()
{
	for ( unsigned int i=0; i<RXI::Controller::Button_Count; ++i )
	{
		RXI::Controller::ButtonID buttonID = static_cast<RXI::Controller::ButtonID>(i);
		if ( mButtons[i] )
			mButtons[i]->setPressed( mRXIController->isButtonPressed(buttonID) );
	}
	
	if ( mLeftStickHorizontalAxis && mLeftStickVerticalAxis )
	{
		SHORT x = 0;
		SHORT y = 0;
		mRXIController->getThumbstickPosition( RXI::Controller::Thumbstick_Left, x, y );
		float normalizedX = (static_cast<float>(x) + 32768) / 65535;
		float normalizedY = (static_cast<float>(y) + 32768) / 65535;
		mLeftStickHorizontalAxis->setNormalizedValue( normalizedX );
		mLeftStickVerticalAxis->setNormalizedValue( normalizedY );
	}
	
	if ( mRightStickHorizontalAxis && mRightStickVerticalAxis )
	{
		SHORT x = 0;
		SHORT y = 0;
		mRXIController->getThumbstickPosition( RXI::Controller::Thumbstick_Right, x, y );
		float normalizedX = (static_cast<float>(x) + 32768) / 65535;
		float normalizedY = (static_cast<float>(y) + 32768) / 65535;
		mRightStickHorizontalAxis->setNormalizedValue( normalizedX );
		mRightStickVerticalAxis->setNormalizedValue( normalizedY );
	}

	if ( mLeftTriggerAxis )
	{
		BYTE position = mRXIController->getTriggerPosition( RXI::Controller::Trigger_Left );
		float normalizedPos = static_cast<float>(position) / 255;
		mLeftTriggerAxis->setNormalizedValue( normalizedPos );
	}

	if ( mRightTriggerAxis )
	{
		BYTE position = mRXIController->getTriggerPosition( RXI::Controller::Trigger_Right );
		float normalizedPos = static_cast<float>(position) / 255;
		mRightTriggerAxis->setNormalizedValue( normalizedPos );
	}

	if ( mBattery )
	{
		BYTE level = mRXIController->getBatteryLevel( RXI::Controller::Battery_Controller );
		float value = static_cast<float>(level) / static_cast<float>(RXI::Controller::getBatteryLevelMax());
		mBattery->setBatteryLevel( value );
	}
}

/*
	ProviderXInput
*/
ProviderXInput::ProviderXInput()
	: mRXIControllerManager(NULL)
{
	mRXIControllerManager = new RXI::ControllerManager();
	mRXIControllerManager->addListener(this);
}

ProviderXInput::~ProviderXInput()
{
	delete mRXIControllerManager;
	mRXIControllerManager = NULL;
}

void ProviderXInput::update()
{
	mRXIControllerManager->update();

	// Update the devices
	const Devices& devices = getDevices();
	for ( Devices::const_iterator itr=devices.begin(); itr!=devices.end(); itr++ )
		(*itr)->update();
}

Device* ProviderXInput::findDeviceFromRXIController( RXI::Controller* rxiController ) const
{
	const Devices& devices = getDevices();
	for ( Devices::const_iterator itr=devices.begin(); itr!=devices.end(); ++itr )
	{
		RXIDevice* rxiDevice = static_cast<RXIDevice*>(*itr);
		if ( rxiDevice->getRXIController()==rxiController)
			return (*itr);
	}
	return NULL;
}

void ProviderXInput::onControllerConnected( RXI::ControllerManager* /*controllerManager*/, RXI::Controller* controller )
{
	std::stringstream stream;
	stream << "XInput Controller #" << controller->getControllerIndex();
	Device* device = new RXIDevice( stream.str(), controller );
	addDevice( device );
}

void ProviderXInput::onControllerDisconnecting( RXI::ControllerManager* /*controllerManager*/, RXI::Controller* controller )
{
	Device* device = findDeviceFromRXIController( controller );	
	assert( device );
	deleteDevice( device );
}

}