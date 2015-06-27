#pragma once

#include "PihaComponent.h"
#include "PihaVector3.h"

namespace Piha
{

class Magnetometer : public Component
{
public:
	Magnetometer( const std::string& name );
	
	virtual bool	start();
	virtual void	stop();
	bool			isStarted() const					{ return mIsStarted; }

	const Vector3d&	getMagneticFieldInGauss() const		{ return mMagneticFieldInGauss; }
	void			setMagneticFieldInGauss( const Vector3d& accelerationInGs );
	
	virtual std::string	toString() const;

protected:
	bool			mIsStarted;
	Vector3d		mMagneticFieldInGauss;
};

}