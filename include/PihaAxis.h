#pragma once

#include "PihaComponent.h"

namespace Piha
{

class Axis : public Component
{
public:
	Axis( const std::string& name, float restValue );
	
	// Returns the raw value of the component normalized to the [0..1] range
	float	getNormalizedValue() const		{ return mNormalizedValue; }
	void	setNormalizedValue( float value );

	// For axis that are mechanically pulled back at rest position, returns
	// this rest position expressed in the [0..1] range. 
	// Returns 0, if there's no concept of rest for the axis (the getNormalizedValue()
	// and getValue() methods will return the same thing)
	float	getRestValue() const			{ return mRestValue; }

	// Returns the axis value expressed relative to the rest position
	float	getValue() const				{ return mNormalizedValue-mRestValue; }
	
	virtual std::string	toString() const;

protected:
	void	setRestValue( float restValue );

	float	mNormalizedValue;
	float	mRestValue;
};

}