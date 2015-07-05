#include "PihaProviderDirectInput.h"

#include <assert.h>
#include "RDICommon.h"
#include "RDIButton.h"
#include "RDIAxis.h"
#include "PihaButton.h"
#include "PihaAxis.h"

namespace Piha
{

/*
	DInputDevice 
	A Piha device that wraps around a RDI::Device
*/
class DInputDevice : public Device
{
public:
	DInputDevice( RDI::Device* dinputDevice );
	
	virtual void			update(); 

	RDI::Device*			getDInputDevice() const { return mDInputDevice; }

private:
	RDI::Device*									mDInputDevice;
	std::vector<std::pair<Button*, RDI::Button*>>	mButtons;	
	std::vector<std::pair<Axis*, RDI::Axis*>>		mAxes;
};

DInputDevice::DInputDevice( RDI::Device* dinputDevice )
	:	Device( RDI::Common::GUIDToString(&dinputDevice->getDeviceInstance().getGuidInstance()), dinputDevice->getDeviceInstance().getInstanceName() ),
		mDInputDevice( dinputDevice ),
		mButtons(),
		mAxes()
{
	const RDI::Objects& dinputObjects = mDInputDevice->getObjects();
	for ( std::size_t i=0; i<dinputObjects.size(); ++i )
	{	
		RDI::Object* dinputObject = dinputObjects[i];
		if ( dynamic_cast<RDI::Button*>( dinputObject ) )
		{
			RDI::Button* dinputButton = static_cast<RDI::Button*>(dinputObject);
			Button* button = new Button( dinputButton->getObjectInstance().getName() );
			mButtons.push_back( std::make_pair( button, dinputButton ) );
			addComponent( button );
		}
		else if ( dynamic_cast<RDI::Axis*>( dinputObject ) )
		{	
			RDI::Axis* dinputAxis = static_cast<RDI::Axis*>(dinputObject);
			Axis* axis = new Axis( dinputAxis->getObjectInstance().getName(), 0.5f );
			mAxes.push_back( std::make_pair( axis, dinputAxis ) );
			addComponent( axis );
		}
	}
}
	
void DInputDevice::update()
{
	for ( std::size_t i=0; i<mButtons.size(); ++i )
	{
		Button* button = mButtons[i].first ;
		RDI::Button* dinputButton = mButtons[i].second;
		button->setPressed( dinputButton->isPressed() );
	}

	for ( std::size_t i=0; i<mAxes.size(); ++i )
	{
		Axis* axis = mAxes[i].first ;
		RDI::Axis* dinputAxis = mAxes[i].second;
		float axisMin = static_cast<float>(dinputAxis->getMinValue());
		float axisMax = static_cast<float>(dinputAxis->getMaxValue());
		float axisValue = static_cast<float>(dinputAxis->getValue());
		assert( axisMin<axisMax );
		assert( axisValue>=axisMin );
		assert( axisValue<=axisMax );

		float normalizedValue = (axisValue-axisMin) / (axisMax-axisMin);
		axis->setNormalizedValue( normalizedValue );
	}
}

/*
	PihaProviderDirectInput
*/
PihaProviderDirectInput::PihaProviderDirectInput( bool ignoreXInputControllers, bool consoleApplication )
	: mDInputControllerManager(NULL)
{
	mDInputControllerManager = new RDI::DeviceManager(ignoreXInputControllers, consoleApplication);
	mDInputControllerManager->addListener(this);
}

PihaProviderDirectInput::~PihaProviderDirectInput()
{
	delete mDInputControllerManager;
	mDInputControllerManager = NULL;
}

void PihaProviderDirectInput::update()
{
	mDInputControllerManager->update();

	// Update the devices
	const Devices& devices = getDevices();
	for ( Devices::const_iterator itr=devices.begin(); itr!=devices.end(); itr++ )
		(*itr)->update();
}

Device* PihaProviderDirectInput::findDeviceFromDInputDevice( RDI::Device* dinputDevice ) const
{
	const Devices& devices = getDevices();
	for ( Devices::const_iterator itr=devices.begin(); itr!=devices.end(); ++itr )
	{
		DInputDevice* device = static_cast<DInputDevice*>(*itr);
		if ( device->getDInputDevice()==dinputDevice)
			return (*itr);
	}
	return NULL;
}

void PihaProviderDirectInput::onDeviceConnected( RDI::DeviceManager* /*dinputDeviceManager*/, RDI::Device* dinputDevice )
{
	Device* device = new DInputDevice( dinputDevice );
	addDevice( device );
}

void PihaProviderDirectInput::onDeviceDisconnecting( RDI::DeviceManager* /*dinputDeviceManager*/, RDI::Device* dinputDevice )
{
	Device* device = findDeviceFromDInputDevice( dinputDevice );	
	assert( device );
	deleteDevice( device );
}

}