#pragma once

#include "PihaComponent.h"
#include "PihaVector3.h"

namespace Piha
{

class Accelerometer : public Component
{
public:
	Accelerometer( const std::string& name );
	
	virtual bool	start();
	virtual void	stop();
	bool			isStarted() const					{ return mIsStarted; }

	const Vector3d&	getAccelerationInGs() const			{ return mAccelerationInGs; }
	void			setAccelerationInGs( const Vector3d& accelerationInGs );
	
	virtual std::string	toString() const;

protected:
	bool			mIsStarted;
	Vector3d		mAccelerationInGs;
};

}