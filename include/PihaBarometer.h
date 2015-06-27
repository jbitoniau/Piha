#pragma once

#include "PihaComponent.h"

namespace Piha
{

class Barometer : public Component
{
public:
	Barometer( const std::string& name );
	
	virtual bool	start();
	virtual void	stop();
	bool			isStarted() const					{ return mIsStarted; }

	float			getPressureInHPa() const	      	{ return mPressureInHPa; }
	void			setPressureInHPa(float pressure);
	
	virtual std::string	toString() const;

protected:
	bool			mIsStarted;
	float   	    mPressureInHPa;
};

}