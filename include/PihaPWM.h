#pragma once

#include "PihaComponent.h"

namespace Piha
{

/*
	PWM

	The PWM class (Pulse Width Modulator) represents a component that
	can gerate a PWM signal. 
	
	This PWM class is designed to drive servos and ESCs (basically
	RC/Hobby-style electronics). The PWM these devices support have the 
	following properties:
	- the pulse rate is 50Hz 
	- the pulse width must be between 1000 and 2000 microseconds 
	
	Note that due to the nature of servos & co, we cannot query the width 
	value from the hardware. The width is (typically) cached internally by 
	the PWM class and updated each time the users sets the pulse width.
	
	This means that at startup we return an initial value (the minimum usually)
	that doesn't correspond to reality. When the first "set pulse width" is done
	successfully, the cached value then reflects the reality. 
	
	Some hardware (like the Pololu Maestro) have a built-in time out system.
	This means that the servo can return to a user-defined neutral value if 
	no pulse width has been sent to the hardware for a certain period of time.
	This can again cause the cached value returned by the PWM component (an 
	instance of this class) to get out of sync with reality.

	References
	http://en.wikipedia.org/wiki/Pulse-width_modulation
	http://www.pololu.com/blog/17/servo-control-interface-in-detail

*/
class PWM : public Component
{
public:
	PWM( const std::string& name );

	// Return the width in microseconds of the pulses currently generated
	// The returned value is guaranteed to be between the allowed min and max 
	unsigned int			getPulseWidthInUs() const;					

	// Set the pulse width (in microseconds)
	// The value must be between the allowed min and max. If not, the value
	// is clamped to the legal range.
	// The method can fail in case of component/device physical problem.
	bool					setPulseWidthInUs( unsigned int widthInUs );

	static unsigned int		getMinPulseWidthInUs() { return 1000; }
	static unsigned int		getMaxPulseWidthInUs() { return 2000; }

	virtual std::string		toString() const;

protected:
	// Implemented by subclasses. The returned value should lay in the legal range
	virtual unsigned int	internalGetPulseWidthInUs() const = 0;					

	// Implemented by subclasses. The width value is guaranteed to be in the legal range 
	virtual bool			internalSetPulseWidthInUs( unsigned int widthInUs ) = 0;	
};

}