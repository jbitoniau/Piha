#include "PihaBarometer.h"

#include <sstream>

namespace Piha
{

Barometer::Barometer( const std::string& name )
	: Component(kBarometer, name),
	  mIsStarted(false),
	  mPressureInHPa(0.f)
{
}

bool Barometer::start()
{
	mIsStarted = true;
	return true;
}

void Barometer::stop()
{
	mIsStarted = false;
}

void Barometer::setPressureInHPa(float pressure)
{
  if (mPressureInHPa == pressure)
		return;
  mPressureInHPa = pressure;
	notifyComponentChanged();
}

std::string	Barometer::toString() const
{
	std::stringstream stream;
	stream << std::fixed;
	stream.precision(2);
  stream << Component::toString() << " pressureInHPa: " << getPressureInHPa();
	return stream.str();
}

}

