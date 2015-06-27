#pragma once

#include "PihaComponent.h"
#include "PihaVector3.h"

namespace Piha
{

/*
	Vibrator
	Starting and stopping the vibrator triggers a "Component Changed" notification
*/
class Vibrator : public Component
{
public:
	Vibrator( const std::string& name, unsigned int maxValue );
	
	unsigned int	getMaxValue() const		{ return mMaxValue; }
	
	unsigned int	getValue() const		{ return mValue; }
	void			setValue( unsigned int value );
	
	float			getNormalizedValue() const;
	void			setNormalizedValue( float value );
	
	virtual std::string	toString() const;

protected:
	void			setMaxValue( unsigned int value ) { mMaxValue = value; }
	
	virtual void	internalSetValue( unsigned int /*value*/ ) {}

private:
	unsigned int	mMaxValue;
	unsigned int	mValue;
};

}