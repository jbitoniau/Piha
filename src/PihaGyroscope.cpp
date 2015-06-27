#include "PihaGyroscope.h"

#include <sstream>

namespace Piha
{

Gyroscope::Gyroscope( const std::string& name )
	: Component(kGyroscope, name),
	  mIsStarted(false),
	  mAngularRateInDegPerSec()
{
}

bool Gyroscope::start()
{
	mIsStarted = true;
	return true;
}

void Gyroscope::stop()
{
	mIsStarted = false;
}

void Gyroscope::setAngularRateInDegPerSec( const Vector3d& angularRateInDegPerSec )
{
	if ( mAngularRateInDegPerSec==angularRateInDegPerSec )
		return;
	mAngularRateInDegPerSec=angularRateInDegPerSec;
	notifyComponentChanged();
}

std::string	Gyroscope::toString() const
{
	std::stringstream stream;
	const Vector3d& ang = getAngularRateInDegPerSec();
	stream << std::fixed;
	stream.precision(3);
	stream << Component::toString() << " angularRateInDegPerSec: x:" << ang.x() << " y:" << ang.y() << " z:" << ang.z();
	return stream.str();
}

}

