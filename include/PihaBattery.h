#pragma once

#include "PihaComponent.h"

namespace Piha
{

class Battery : public Component
{
public:
	Battery( const std::string& name );
	
	// Returns the battery level between 0 (empty) and 1 (fully charged)
	float	getBatteryLevel() const				{ return mBatteryLevel; }
	void	setBatteryLevel( float value );

	virtual std::string	toString() const;

protected:
	float	mBatteryLevel;
};

}