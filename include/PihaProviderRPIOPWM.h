#pragma once

#include "PihaDeviceProvider.h"
#include <vector>

namespace Piha
{

class RPIOPWMDevice;

class ProviderRPIOPWM : public DeviceProvider
{
public:
	ProviderRPIOPWM( const std::vector<int>& gpios, unsigned int channelSubcycleTimeInUs );
	virtual ~ProviderRPIOPWM();

	void			update();

	static int 		getHardware();
	int 			getChannelSubcycleTimeInUs() const;
	static int 		getPulseWidthIncrementInUs();
	static int 		getChannelIndex();
	
private:
	RPIOPWMDevice*	mRPIOPWMDevice;
	unsigned int	mChannelSubcycleTimeInUs;
};

}