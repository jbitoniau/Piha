#include "PihaAxis.h"

#include <assert.h>
#include <sstream>

namespace Piha
{

Axis::Axis( const std::string& name, float restValue )
	: Component(kAxis, name),
	  mNormalizedValue(0.f),
	  mRestValue(0.f)
{
	setRestValue( restValue );
	setNormalizedValue( restValue );
}

void Axis::setRestValue( float restValue ) 
{
	assert( restValue>=0.f && restValue<=1.f );
	if ( restValue<0.f )
		restValue = 0.f;
	else if ( restValue>1.f )
		restValue = 1.f;
	mRestValue = restValue;
}

void Axis::setNormalizedValue( float value )
{
	assert( value>=0.f && value<=1.f );
	if ( value<0.f )
		value = 0.f;
	else if ( value>1.f )
		value = 1.f;
	if ( mNormalizedValue==value )
		return;
	mNormalizedValue=value;
	notifyComponentChanged();
}
	
std::string	Axis::toString() const
{
	std::stringstream stream;
	stream << Component::toString();
	stream << std::fixed;
	stream.precision(3);
	stream << " ";
	stream << "normalizedValue: " << getNormalizedValue() << " ";
	stream << "restValue: " << getRestValue() << " ";
	stream << "(value: " << getValue() << ")";
	return stream.str();
}

}

