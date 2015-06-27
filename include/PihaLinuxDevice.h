#pragma once

#include "PihaDevice.h"
#include "PihaDeviceProvider.h"

namespace Piha
{

class LinuxDevice :	public Device,
					public DeviceProvider::Listener
{
public:
	LinuxDevice(/*unsigned int pwmChannelSubcycleTimeInUs=20000*/);  // Note: a bit ugly, but will do for now
	virtual ~LinuxDevice();

	virtual void update();

protected:
	virtual void onDeviceAdded( DeviceProvider* deviceProvider, Device* device );
	virtual void onDeviceRemoving( DeviceProvider* deviceProvider, Device* device );

private:
	std::vector<DeviceProvider*> mDeviceProviders;
};

}