#include "PihaTemperatureSensor.h"

#include <sstream>

namespace Piha
{

TemperatureSensor::TemperatureSensor( const std::string& name )
	: Component(kTemperatureSensor, name),
	  mIsStarted(false),
	  mTemperatureInDegreeCelsius(0.f)
{
}

bool TemperatureSensor::start()
{
	mIsStarted = true;
	return true;
}

void TemperatureSensor::stop()
{
	mIsStarted = false;
}

void TemperatureSensor::setTemperatureInDegreeCelsius( float temperature )
{
  if (mTemperatureInDegreeCelsius == temperature)
		return;
  mTemperatureInDegreeCelsius = temperature;
	notifyComponentChanged();
}

std::string	TemperatureSensor::toString() const
{
	std::stringstream stream;
	stream << std::fixed;
	stream.precision(2);
	stream << Component::toString() << " temperatureInDegreeCelsius: " << getTemperatureInDegreeCelsius();
	return stream.str();
}

}

