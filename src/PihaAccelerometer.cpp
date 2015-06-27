#include "PihaAccelerometer.h"

#include <sstream>

namespace Piha
{

/*
	Notes:
	- Maybe the Accelerometer (as for all the other Components) should be an abstract class where start and stop are necessarily
	 implemented by the subclass. What is the meaning of an Accelerometer class with start/stop that only changes a boolean?
*/

Accelerometer::Accelerometer( const std::string& name )
	: Component(kAccelerometer, name),
	  mIsStarted(false),
	  mAccelerationInGs()
{
}

bool Accelerometer::start()
{
	mIsStarted = true;
	return true;
}

void Accelerometer::stop()
{
	mIsStarted = false;
}

void Accelerometer::setAccelerationInGs( const Vector3d& accelerationInGs )
{
	if ( mAccelerationInGs==accelerationInGs )
		return;
	mAccelerationInGs=accelerationInGs;
	notifyComponentChanged();
}

std::string	Accelerometer::toString() const
{
	std::stringstream stream;
	const Vector3d& acc = getAccelerationInGs();
	stream << std::fixed;
	stream.precision(3);
	stream << Component::toString() << " accelerationInGs: x:" << acc.x() << " y:" << acc.y() << " z:" << acc.z();
	return stream.str();
}

}

