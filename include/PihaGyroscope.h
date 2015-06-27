#pragma once

#include "PihaComponent.h"
#include "PihaVector3.h"

namespace Piha
{

class Gyroscope : public Component
{
public:
	Gyroscope( const std::string& name );
	
	virtual bool	start();
	virtual void	stop();
	bool			isStarted() const					{ return mIsStarted; }

	const Vector3d&	getAngularRateInDegPerSec() const	{ return mAngularRateInDegPerSec; }
	void			setAngularRateInDegPerSec( const Vector3d& angularRateInDegPerSec );
	
	virtual std::string	toString() const;

protected:
	bool			mIsStarted;
	Vector3d		mAngularRateInDegPerSec;
};

}