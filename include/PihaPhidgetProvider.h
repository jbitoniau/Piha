#pragma once

#include "PihaDeviceProvider.h"
#include "RPhiLocalDeviceManager.h"

namespace Piha
{

class PhidgetProvider :	public DeviceProvider,
						public RPhi::DeviceManager::Listener
{
public:
	PhidgetProvider();
	virtual ~PhidgetProvider();

	void					update();

protected:
	Device*					findDeviceFromRPhiDevice( RPhi::Device* rphiDevice ) const;

	virtual void			onDeviceConnected( RPhi::DeviceManager* deviceManager, RPhi::Device* device );
	virtual void			onDeviceDisconnecting( RPhi::DeviceManager* deviceManager, RPhi::Device* device );
	
private:
	RPhi::DeviceManager*	mRPhiDeviceManager;
};

}