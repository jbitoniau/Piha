#pragma once

#include "PihaDevice.h"
#include "PihaDeviceProvider.h"

namespace Piha
{

class WindowsDevice :	public Device,
						public DeviceProvider::Listener
{
public:
	WindowsDevice( bool consoleApplication );
	virtual ~WindowsDevice();

	virtual void update();

protected:
	virtual void onDeviceAdded( DeviceProvider* deviceProvider, Device* device );
	virtual void onDeviceRemoving( DeviceProvider* deviceProvider, Device* device );

private:
	std::vector<DeviceProvider*> mDeviceProviders;
};

}