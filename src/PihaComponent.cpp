#include "PihaComponent.h"

#include "PihaDevice.h"
#include <sstream>

namespace Piha
{

const char*	Component::mTypeNames[kNumTypes] = 
{
	"accelerometer",
	"gyroscope",
	"magnetometer",
	"temperature sensor",
	"barometer",
	"GPS receiver",
	"button",
	"axis",
	"vibrator",
	"battery",
	"camera",
	"PWM"
};

Component::Component( Type type, const std::string& name ) 
	: mParentDevice(NULL),
	  mType(type),
	  mName(name)
{
}

Component::~Component()
{
}

const char*	Component::getTypeName( Type type )
{
	return mTypeNames[type];
}
	
void Component::setParentDevice( Device* parentDevice, std::size_t indexInParentDevice )
{ 
	mParentDevice=parentDevice; 
	mIndexInParentDevice=indexInParentDevice; 
}

void Component::notifyComponentChanged()
{
	if ( getParentDevice() )
		getParentDevice()->notifyComponentChanged( this );
}

std::string	Component::toString() const
{
	std::stringstream stream;
	stream << "component: type:" << getTypeName() << " name:'" << getName() << "'";
	return stream.str();
}

}
