#include "PihaPWM.h"

#include <sstream>

namespace Piha
{

PWM::PWM( const std::string& name )
	: Component(kPWM, name)
{
}

unsigned int PWM::getPulseWidthInUs() const
{
	unsigned int width = internalGetPulseWidthInUs();
	if ( width<getMinPulseWidthInUs() )
		width = getMinPulseWidthInUs();			// Report that something's wrong at lower level somehow
	else if ( width>getMaxPulseWidthInUs() )
		width = getMaxPulseWidthInUs();			// Report that something's wrong at lower level somehow
	return width;
}

bool PWM::setPulseWidthInUs( unsigned int widthInUs )		// Should this take care of checking widthInUs against getPulseWidthInUs() and return if same?
{															// Also notifying when the change happened ok?
	unsigned int width = widthInUs;
	if ( width<getMinPulseWidthInUs() )
		width = getMinPulseWidthInUs();			// Report that something's wrong at lower level somehow
	else if ( width>getMaxPulseWidthInUs() )
		width = getMaxPulseWidthInUs();			// Report that something's wrong at lower level somehow
	return internalSetPulseWidthInUs( width );
}

std::string	PWM::toString() const
{
	std::stringstream stream;
	stream << Component::toString() << " pulseWidthInUs: " << getPulseWidthInUs();
	return stream.str();
}

}

