#include "PihaLinuxDevice.h"

#include <assert.h>
#include "PihaConfig.h"

#ifdef PIHA_I2CDEVLIB_SUPPORT
	#include "PihaProviderI2CDevLib.h"
#endif
#ifdef PIHA_LINUXJOYSTICK_SUPPORT
	#include "PihaLinuxJoystickProvider.h"
#endif
#ifdef PIHA_PHIDGET_SUPPORT
	#include "PihaPhidgetProvider.h"
#endif
#ifdef PIHA_POLOLUMAESTRO_SUPPORT
	#include "PihaPololuMaestroProvider.h"
#endif
#ifdef PIHA_RPIOPWM_SUPPORT
	#include "PihaProviderRPIOPWM.h"
#endif

namespace Piha
{

LinuxDevice::LinuxDevice(/*unsigned int pwmChannelSubcycleTimeInUs*/)
	: Device("Linux device", "Linux device"),
	  mDeviceProviders()
{
	DeviceProvider* provider = NULL;

#ifdef PIHA_I2CDEVLIB_SUPPORT
	ProviderI2CDevLib::I2cDeviceIdentifiers deviceIdentifiers;	
	deviceIdentifiers.push_back( ProviderI2CDevLib::I2cDeviceIdentifier( ProviderI2CDevLib::kMPU6050, 0x69 /*105*/) );
	deviceIdentifiers.push_back( ProviderI2CDevLib::I2cDeviceIdentifier( ProviderI2CDevLib::kHMC5883L, 0x1E /*30*/) );
	deviceIdentifiers.push_back( ProviderI2CDevLib::I2cDeviceIdentifier( ProviderI2CDevLib::kMS561101BA, 0x77 /*119*/) );
	provider = new ProviderI2CDevLib( "/dev/i2c-1", deviceIdentifiers);
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif

#ifdef PIHA_LINUXJOYSTICK_SUPPORT
    provider = new LinuxJoystickProvider("/dev/input/js", 5);
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif

#ifdef PIHA_PHIDGET_SUPPORT
	provider = new PhidgetProvider();
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif
			
#ifdef PIHA_POLOLUMAESTRO_SUPPORT
	provider = new PololuMaestroProvider(6, "/dev/ttyACM0", 9600);
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif

#ifdef PIHA_RPIOPWM_SUPPORT
	unsigned int pwmChannelSubcycleTimeInUs	= 20000;		// Hard coded!
	std::vector<int> gpios;
	//gpios.push_back( 23 );
	gpios.push_back( 24 );
	gpios.push_back( 25 );
    provider = new ProviderRPIOPWM(gpios, pwmChannelSubcycleTimeInUs);
	provider->addListener(this);
	mDeviceProviders.push_back( provider );
#endif
}

LinuxDevice::~LinuxDevice()
{
	for ( std::size_t i=0; i<mDeviceProviders.size(); ++i )
		delete mDeviceProviders[i];
	mDeviceProviders.clear();
	assert( getChildDevices().empty() );
}

void LinuxDevice::update()
{
	for ( std::size_t i=0; i<mDeviceProviders.size(); ++i )
		mDeviceProviders[i]->update();
}

void LinuxDevice::onDeviceAdded( DeviceProvider* deviceManager, Device* device )
{
	addChildDevice( device );
}

void LinuxDevice::onDeviceRemoving( DeviceProvider* deviceManager, Device* device )
{
	removeChildDevice( device );
}

}
