#pragma once

#include <string>
#include <vector>

namespace Piha
{

class Device;

class Component
{
public:
	enum Type
	{
		kAccelerometer,
		kGyroscope,
		kMagnetometer,
		kTemperatureSensor,
		kBarometer,
		kGPSReceiver,
		kButton,
		kAxis,
		kVibrator,
		kBattery,
		kCamera,
		kPWM,
		kNumTypes
	};

	Component( Type type, const std::string& name );
	virtual ~Component();

	std::size_t			getIndexInParentDevice()				{ return mIndexInParentDevice; }
	Type				getType() const							{ return mType; }
	const char*			getTypeName() const						{ return getTypeName(getType()); }
	static const char*	getTypeName( Type type );
		
	const std::string&	getName() const							{ return mName; }
	Device*				getParentDevice() const					{ return mParentDevice; }
	
	virtual std::string	toString() const;

protected:
	friend class Device;
	void				setParentDevice( Device* parentDevice, std::size_t indexInParentDevice );
	void				notifyComponentChanged();

private:
	static const char*	mTypeNames[kNumTypes];
	Device*				mParentDevice;
	std::size_t			mIndexInParentDevice;
	Type				mType;
	std::string			mName;
};

typedef std::vector<Component*> Components;
	

}