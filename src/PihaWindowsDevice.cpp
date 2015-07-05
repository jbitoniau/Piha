#include "PihaWindowsDevice.h"

#include <assert.h>
#include "PihaConfig.h"

#ifdef PIHA_DIRECTINPUT_SUPPORT
	#include "PihaProviderDirectInput.h"
#endif
#ifdef PIHA_DIRECTSHOW_SUPPORT
	#include "PihaDirectShowProvider.h"
#endif
#ifdef PIHA_PHIDGET_SUPPORT
	#include "PihaPhidgetProvider.h"
#endif
#ifdef PIHA_POLOLUMAESTRO_SUPPORT
	#include "PihaPololuMaestroProvider.h"
#endif
#ifdef PIHA_XINPUT_SUPPORT
	#include "PihaProviderXInput.h"
#endif

namespace Piha
{

WindowsDevice::WindowsDevice( bool consoleApplication )
	: Device("Windows Device", "Windows device"),
	  mDeviceProviders()
{
	DeviceProvider* provider = NULL;
#ifdef PIHA_DIRECTINPUT_SUPPORT
	provider = new PihaProviderDirectInput(true, consoleApplication);
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif
	
#ifdef PIHA_DIRECTSHOW_SUPPORT
	provider = new DirectShowProvider();
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif
		
#ifdef PIHA_PHIDGET_SUPPORT
	provider = new PhidgetProvider();
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif
			
#ifdef PIHA_POLOLUMAESTRO_SUPPORT
	provider = new PololuMaestroProvider(6, "COM4", 9600);
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif

#ifdef PIHA_XINPUT_SUPPORT
	provider = new ProviderXInput();
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif
}

WindowsDevice::~WindowsDevice()
{
	for ( std::size_t i=0; i<mDeviceProviders.size(); ++i )
		delete mDeviceProviders[i];
	mDeviceProviders.clear();
	assert( getChildDevices().empty() );
}

void WindowsDevice::update()
{
	for ( std::size_t i=0; i<mDeviceProviders.size(); ++i )
		mDeviceProviders[i]->update();
}

void WindowsDevice::onDeviceAdded( DeviceProvider* /*deviceProvider*/, Device* device )
{
	addChildDevice( device );
}

void WindowsDevice::onDeviceRemoving( DeviceProvider* /*deviceProvider*/, Device* device )
{
	removeChildDevice( device );
}

}