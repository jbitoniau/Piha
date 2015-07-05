#pragma once

#include "PihaDeviceProvider.h"
#include "RDIDeviceManager.h"
#include <map>

namespace Piha
{

class PihaProviderDirectInput :	public DeviceProvider,
								public RDI::DeviceManager::Listener
{
public:
	PihaProviderDirectInput( bool ignoreXInputControllers, bool consoleApplication );
	virtual ~PihaProviderDirectInput();

	void			update();

protected:
	Device*			findDeviceFromDInputDevice( RDI::Device* dinputDevice ) const;
	virtual void	onDeviceConnected( RDI::DeviceManager* deviceManager, RDI::Device* device );
	virtual void	onDeviceDisconnecting( RDI::DeviceManager* deviceManager, RDI::Device* device );

private:
	RDI::DeviceManager*	mDInputControllerManager;
};

}