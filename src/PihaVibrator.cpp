#include "PihaVibrator.h"

#include <sstream>
#include <cmath>

namespace Piha
{

Vibrator::Vibrator( const std::string& name, unsigned int maxValue  )
	: Component(kVibrator, name),
	  mMaxValue(0),
	  mValue(0)
{
	setMaxValue(maxValue);
}

void Vibrator::setValue( unsigned int value )
{
	if ( value>mMaxValue )
		value=mMaxValue;
	if ( value==mValue )
		return;
	internalSetValue( value );
	mValue = value;
	notifyComponentChanged();
}

float Vibrator::getNormalizedValue() const
{
	float v = 0.f;
	if ( getMaxValue()!=0.f )
		v = static_cast<float>( getValue() ) /  static_cast<float>( getMaxValue() );
	return v;
}

void Vibrator::setNormalizedValue( float value )
{
	if ( value>1.f )
		value = 1.f;
	else if ( value<0.f )
		value = 0.f;
	float v = floor( value*getMaxValue() );
	setValue( static_cast<unsigned int>(v) );
}

std::string	Vibrator::toString() const
{
	std::stringstream stream;
	stream << Component::toString() << " Value:" << getValue() << " MaxValue:" << getMaxValue();
	return stream.str();
}

}

