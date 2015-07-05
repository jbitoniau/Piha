#pragma once

#include "PihaDeviceProvider.h"
#include <vector>

namespace Piha
{

class ProviderI2CDevLib : public DeviceProvider
{
public:
	enum I2cDeviceType
	{
		kBMP085,
		kMPU6050,
		kHMC5883L,
		kMS561101BA
	};

	// Defines a I2C device to manage using a type and an address
	class I2cDeviceIdentifier
	{
	public:
		// Use address 0 to use the device's default address. The default adress resolution 
		// is done here in the constructor. Calls to getAddress() will return resolved address
		I2cDeviceIdentifier( I2cDeviceType type, unsigned char address=0 );

		I2cDeviceType getType() const				{ return mType; }
		unsigned char getAddress() const			{ return mAddress; }
		unsigned char getDefaultAddress() const		{ return getDefaultAddress( getType() ); }
		static unsigned char getDefaultAddress( I2cDeviceType type );

	private:
		I2cDeviceType mType;
		unsigned char mAddress;
	};

	typedef std::vector<I2cDeviceIdentifier> I2cDeviceIdentifiers;
	
	ProviderI2CDevLib( const std::string& i2cFilename, const I2cDeviceIdentifiers& i2cDeviceIdentifiers );
	virtual ~ProviderI2CDevLib();

	void	update();

private:
	std::vector<Device*> mDevices;
};

}