#include "PihaBattery.h"

#include <assert.h>
#include <sstream>

namespace Piha
{

Battery::Battery( const std::string& name )
	: Component(kBattery, name),
	  mBatteryLevel(0.f)
{
}

void Battery::setBatteryLevel( float value )
{
	assert( value>=0.f && value<=1.f );
	if ( value<0.f )
		value = 0.f;
	else if ( value>1.f )
		value = 1.f;
	mBatteryLevel = value;
	notifyComponentChanged();
}
	
std::string	Battery::toString() const
{
	std::stringstream stream;
	stream << Component::toString();
	stream << std::fixed;
	stream.precision(3);
	stream << " ";
	stream << "batteryLevel: " << getBatteryLevel() << " ";
	return stream.str();
}

}

