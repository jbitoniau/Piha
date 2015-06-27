#pragma once

#include "PihaComponent.h"

namespace Piha
{

class TemperatureSensor : public Component
{
public:
	TemperatureSensor( const std::string& name );
	
	virtual bool	start();
	virtual void	stop();
	bool			isStarted() const				              	{ return mIsStarted; }

	float			getTemperatureInDegreeCelsius() const		{ return mTemperatureInDegreeCelsius; }
	void			setTemperatureInDegreeCelsius( float temperature );
	
	virtual std::string	toString() const;

protected:
	bool			mIsStarted;
	float   		mTemperatureInDegreeCelsius;
};

}