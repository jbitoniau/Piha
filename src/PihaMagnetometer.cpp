#include "PihaMagnetometer.h"

#include <sstream>

namespace Piha
{

Magnetometer::Magnetometer( const std::string& name )
	: Component(kMagnetometer, name),
	  mIsStarted(false),
	  mMagneticFieldInGauss()
{
}

bool Magnetometer::start()
{
	mIsStarted = true;
	return true;
}

void Magnetometer::stop()
{
	mIsStarted = false;
}

void Magnetometer::setMagneticFieldInGauss( const Vector3d& magneticFieldInGauss )
{
	if ( mMagneticFieldInGauss==magneticFieldInGauss )
		return;
	mMagneticFieldInGauss=magneticFieldInGauss;
	notifyComponentChanged();
}

std::string	Magnetometer::toString() const
{
	std::stringstream stream;
	const Vector3d& mag = getMagneticFieldInGauss();
	stream << std::fixed;
	stream.precision(3);
	stream << Component::toString() << " magneticFieldInGauss: x:" << mag.x() << " y:" << mag.y() << " z:" << mag.z();
	return stream.str();
}

}

