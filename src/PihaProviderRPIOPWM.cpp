#include "PihaProviderRPIOPWM.h"

#include "PihaPWM.h"

extern "C" 
{
	#include "pwm.h"
}

#include <sstream>
#include <cstdlib>
#include <stdio.h>	// printf
//#include "PihaTime.h"

namespace Piha
{

/*
	RPIOPWM
*/
class RPIOPWM : public PWM
{
public:
	RPIOPWM( const std::string& name, int gpio, int channelIndex );
	virtual ~RPIOPWM();
	
protected:
	virtual unsigned int	internalGetPulseWidthInUs() const;
	virtual bool			internalSetPulseWidthInUs( unsigned int widthInUs );

private:
	int						mGPIO;
	int 					mChannelIndex;
	unsigned int			mPulseWidth;
};

RPIOPWM::RPIOPWM( const std::string& name, int gpio, int channelIndex )
	: PWM(name),
	  mGPIO(gpio),
	  mChannelIndex(channelIndex),
	  mPulseWidth(PWM::getMinPulseWidthInUs())
{
	internalSetPulseWidthInUs(mPulseWidth);
}

RPIOPWM::~RPIOPWM()
{
}

unsigned int RPIOPWM::internalGetPulseWidthInUs() const
{
	return mPulseWidth;
}

bool RPIOPWM::internalSetPulseWidthInUs( unsigned int widthInUs )
{
	//clear_channel_gpio(mPWMIndex, mGPIO);		
	int widthInIncrements = widthInUs / ProviderRPIOPWM::getPulseWidthIncrementInUs();
	int ret = add_channel_pulse( mChannelIndex, mGPIO, 0, widthInIncrements);
	if ( ret!=EXIT_SUCCESS )
	{
		printf("Problem internalSetPulseWidthInUs: %s\n", get_error_message());
		return false;
	}
	mPulseWidth = widthInUs;
	notifyComponentChanged();
	return true;
}

/*
	RPIOPWMDevice
*/
class RPIOPWMDevice : public Device
{
public:
	RPIOPWMDevice( const std::vector<int>& gpios, unsigned int channelSubcycleTimeInUs );
	virtual ~RPIOPWMDevice();

	virtual void update(); 

protected:
	static bool isValidGPIO( int gpio );
		
private:
	static const int mNumValidGPIOs = 8;
	static int mValidGPIOs[mNumValidGPIOs];
};

// 8 GPIOs to use for driving servos
int RPIOPWMDevice::mValidGPIOs[] = {
	//GPIO#   Pin#
    4,     // P1-7
    17,    // P1-11
    18,    // P1-12
    21,    // P1-13
    22,    // P1-15
    23,    // P1-16
    24,    // P1-18
    25,    // P1-22
};

RPIOPWMDevice::RPIOPWMDevice( const std::vector<int>& gpios, unsigned int channelSubcycleTimeInUs )
	: Device("GPIO PWMs", "GPIO PWMs")
{
	int channelIndex = ProviderRPIOPWM::getChannelIndex();
	int ret = init_channel(channelIndex, channelSubcycleTimeInUs);		
	printf("RPIOPWM: channelSubcycleTimeInUs set to %d us\n", channelSubcycleTimeInUs);
	if ( ret!=EXIT_SUCCESS )
	{
		// Problem!
		printf("Problem RPIOPWM: %s\n", get_error_message());
	}
	
	for ( std::size_t i=0; i<gpios.size(); ++i )
	{
		int gpio = gpios[i];
		if ( isValidGPIO(gpio) )
		{
			std::stringstream stream;
			stream << "GPIO" << gpio;
			PWM* pwm = new RPIOPWM( stream.str(), gpio, channelIndex );
			addComponent( pwm );
		}
	}
}

RPIOPWMDevice::~RPIOPWMDevice()
{
}

void RPIOPWMDevice::update()
{
}

bool RPIOPWMDevice::isValidGPIO( int gpio )
{
	for ( int i=0; i<mNumValidGPIOs; ++i )
		if ( mValidGPIOs[i]==gpio )
			return true;
	return false;
}
	
/*
	ProviderRPIOPWM
*/	
ProviderRPIOPWM::ProviderRPIOPWM( const std::vector<int>& gpios, unsigned int channelSubcycleTimeInUs )
	: mRPIOPWMDevice(NULL),
	  mChannelSubcycleTimeInUs(channelSubcycleTimeInUs)
{
	// Fatal errors in the rpio-pwm lib will not cause it to exit the program
	// The function will simply return EXIT_FAILURE and a description of the
	// problem can be obtained using get_error_message()
	set_softfatal(1);
	
	// Only report errors (not simple debug messages)
	set_loglevel(LOG_LEVEL_ERRORS);
	
	int ret = setup(getPulseWidthIncrementInUs(), getHardware());
	if ( ret!=EXIT_SUCCESS )
	{
		// Problem!
		printf("Problem ProviderRPIOPWM: %s\n", get_error_message());
	}

	// Declaring/adding the device to Piha of the device takes place at the first update. 
	// This allows Piha and its client code to be notified of the new device existence
	mRPIOPWMDevice = new RPIOPWMDevice( gpios, getChannelSubcycleTimeInUs() );
}

ProviderRPIOPWM::~ProviderRPIOPWM()
{
	shutdown();
}

void ProviderRPIOPWM::update()
{
	if ( getDevices().empty() )
		addDevice( mRPIOPWMDevice );
	
	 mRPIOPWMDevice->update();
}

int ProviderRPIOPWM::getHardware()
{ 	
	return DELAY_VIA_PWM; 
}

int ProviderRPIOPWM::getChannelSubcycleTimeInUs() const
{ 
	return mChannelSubcycleTimeInUs;
}

int ProviderRPIOPWM::getPulseWidthIncrementInUs() 	
{ 
	return 10; 
}	

int ProviderRPIOPWM::getChannelIndex() 	
{ 
	return 0; 
}	

} 
