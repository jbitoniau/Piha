#pragma once

#include "PihaDeviceProvider.h"
#include "RXIControllerManager.h"

namespace Piha
{

class ProviderXInput :	public DeviceProvider,
						public RXI::ControllerManager::Listener
{
public:
	ProviderXInput();
	virtual ~ProviderXInput();

	void					update();

protected:
	Device*					findDeviceFromRXIController( RXI::Controller* rxiController ) const;
	virtual void			onControllerConnected( RXI::ControllerManager* controllerManager, RXI::Controller* controller );
	virtual void			onControllerDisconnecting( RXI::ControllerManager* controllerManager, RXI::Controller* controller );
		
private:
	RXI::ControllerManager* mRXIControllerManager;
};

}