#include "PihaProviderI2CDevLib.h"

#include "PihaTemperatureSensor.h"
#include "PihaBarometer.h"
#include "PihaAccelerometer.h"
#include "PihaGyroscope.h"
#include "PihaMagnetometer.h"

//#include <sstream>
//#include <cstdlib>
#include <stdio.h>	// printf
#include <iostream>	// Debug!
//#include "PihaTime.h"

#include "MPU6050/MPU6050.h"
#include "BMP085/BMP085.h"
#include "HMC5883L/HMC5883L.h"
#include "MS561101BA/MS561101BA.h"

#include <sstream>
#include <stdlib.h>

namespace Piha
{

/*
	ProviderI2CDevLib::I2cDeviceIdentifier
*/
ProviderI2CDevLib::I2cDeviceIdentifier::I2cDeviceIdentifier( I2cDeviceType type, unsigned char address )
	: mType(type), 
	  mAddress(address) 
{
	if ( address==0 )
		mAddress = getDefaultAddress();
}

unsigned char ProviderI2CDevLib::I2cDeviceIdentifier::getDefaultAddress( ProviderI2CDevLib::I2cDeviceType type )
{
	switch ( type )
	{
		case kBMP085:		return BMP085_DEFAULT_ADDRESS;
		case kMPU6050:		return MPU6050_DEFAULT_ADDRESS;
		case kHMC5883L:		return HMC5883L_DEFAULT_ADDRESS;
		case kMS561101BA:	return MS561101BA_DEFAULT_ADDRESS;
	}
	return 0;		// Problem! Need a default address for all types of I2C Devices. That's a bug!
}
	
/*
	HMC5883LDevice
*/
class HMC5883LDevice : public Device
{
public:	
	HMC5883LDevice( const std::string& pihaIdentifier, uint8_t address );
	virtual ~HMC5883LDevice();
	
	virtual void update();

private:
	HMC5883L* mHMC5883L;
};

HMC5883LDevice::HMC5883LDevice( const std::string& pihaIdentifier, uint8_t address )
	: Device(pihaIdentifier, "HMC5883L"),
	  mHMC5883L(NULL)
{
	mHMC5883L = new HMC5883L(address);
	mHMC5883L->initialize();
	
	if (mHMC5883L->testConnection())
	{
    // Note: the initial setting of the encapsulated I2C device should happen here
    Magnetometer* magnetometer = new Magnetometer("");
		addComponent(magnetometer);
	}
	else
	{
		printf("Failed to connect to HMC5883L at %d\n", address);		// !!!!
		delete mHMC5883L;
		mHMC5883L = NULL;
	}
}

HMC5883LDevice::~HMC5883LDevice()
{
	delete mHMC5883L;
	mHMC5883L = NULL;
}

void HMC5883LDevice::update()
{
	if (!mHMC5883L)
		return;

	Magnetometer* magnetometer = static_cast<Magnetometer*>(getComponents()[0]);
	if (magnetometer->isStarted())
	{
		int16_t mx = 0;
		int16_t my = 0;
		int16_t mz = 0;
		mHMC5883L->getHeading(&mx, &my, &mz);
		magnetometer->setMagneticFieldInGauss( Vector3d(mx, my, mz) );
	}
	else
	{
		magnetometer->setMagneticFieldInGauss( Vector3d() );
	}
}

/*
	BMP085Device
*/
class BMP085Device : public Device
{
public:
	BMP085Device( const std::string& pihaIdentifier, uint8_t address );
	virtual ~BMP085Device();

	virtual void update();
	
private:
	BMP085*	mBMP085;
};

BMP085Device::BMP085Device( const std::string& pihaIdentifier, uint8_t address )
	: Device(pihaIdentifier, "BMP085"),
   	  mBMP085(NULL)
{
	mBMP085 = new BMP085(address);	
	mBMP085->initialize();
	
	if (mBMP085->testConnection())
	{
    // Note: the initial setting of the encapsulated I2C device should happen here
    TemperatureSensor* temperatureSensor = new TemperatureSensor("");
		addComponent(temperatureSensor);
		Barometer* barometer = new Barometer("");
		addComponent(barometer);
	}
	else
	{
		printf("Failed to connect to BMP085 at %d\n", address);		// !!!!
		delete mBMP085;
		mBMP085 = NULL;
	}
}

BMP085Device::~BMP085Device()
{
	delete mBMP085;
	mBMP085 = NULL;
}

// Note: this update can potentially take a long time. It should be done in a separate thread
void BMP085Device::update()
{
	if (!mBMP085)
		return;

	TemperatureSensor* temperatureSensor = static_cast<TemperatureSensor*>(getComponents()[0]);
	if (temperatureSensor->isStarted())
	{
		mBMP085->setControl(BMP085_MODE_TEMPERATURE);
		uint16_t delay = mBMP085->getMeasureDelayMicroseconds();
		usleep(delay);
		float temperature = mBMP085->getTemperatureC();
		temperatureSensor->setTemperatureInDegreeCelsius(temperature);
	}
	else
	{
		temperatureSensor->setTemperatureInDegreeCelsius(0.f);
	}

	Barometer* barometer = static_cast<Barometer*>(getComponents()[1]);
	if (barometer->isStarted())
	{
		mBMP085->setControl(BMP085_MODE_PRESSURE_3);
		uint16_t delay = mBMP085->getMeasureDelayMicroseconds();
		usleep(delay);
		float pressureInPa = mBMP085->getPressure();
		barometer->setPressureInHPa(pressureInPa / 100.f);
	}	
	else
	{
		barometer->setPressureInHPa(0.f);
	}
}



/*
	MS561101BADevice
*/
class MS561101BADevice : public Device
{
public:
	MS561101BADevice( const std::string& pihaIdentifier, uint8_t address );
	virtual ~MS561101BADevice();

	virtual void update();
	
private:
	MS561101BA*	mMS561101BA;
};

MS561101BADevice::MS561101BADevice( const std::string& pihaIdentifier, uint8_t address )
	: Device(pihaIdentifier, "MS5611-01BA"),
   	  mMS561101BA(NULL)
{
	mMS561101BA = new MS561101BA(address);	
	mMS561101BA->initialize();      // Note: this is costly on the MS5611-01BA. See if we can decrease the delay
	
	if (mMS561101BA->testConnection())
	{
    // Note: the initial setting of the encapsulated I2C device should happen here
    TemperatureSensor* temperatureSensor = new TemperatureSensor("");
		addComponent(temperatureSensor);
		Barometer* barometer = new Barometer("");
		addComponent(barometer);
	}
	else
	{
		printf("Failed to connect to MS5611-01BA at %d\n", address);		// !!!!
		delete mMS561101BA;
		mMS561101BA = NULL;
	}
}

MS561101BADevice::~MS561101BADevice()
{
	delete mMS561101BA;
	mMS561101BA = NULL;
}

void MS561101BADevice::update()
{
	if (!mMS561101BA)
		return;

	TemperatureSensor* temperatureSensor = static_cast<TemperatureSensor*>(getComponents()[0]);
	Barometer* barometer = static_cast<Barometer*>(getComponents()[1]);
	float temperature = 0.f;
	float pressure = 0.f;
	if (temperatureSensor->isStarted() || barometer->isStarted())
	{
		if ( !mMS561101BA->readValues( &pressure, &temperature ) )
		{
			// Failed to read the values!
			temperature = 0.f;
			pressure = 0.f;
		}
	}

	if ( temperatureSensor->isStarted() )
		temperatureSensor->setTemperatureInDegreeCelsius(temperature);
	else
		temperatureSensor->setTemperatureInDegreeCelsius(0.f);
	
	if ( barometer->isStarted() )
		barometer->setPressureInHPa(pressure);
	else
		barometer->setPressureInHPa(0.f);	
}

/*
	MPU6050Device
*/
class MPU6050Device : public Device
{
public:
	MPU6050Device( const std::string& pihaIdentifier, uint8_t address );
	virtual ~MPU6050Device();
	
	bool sendCommand( const std::string& command, std::string& result );
	
	virtual void update(); 

protected:
	double 		getAccelerometerHalfScaleRange() const;
	double		getGyroscopeHalfScaleRange() const;
	
private:
	MPU6050*	mMPU6050;
	
	// The accelerometer can be configured to return a g value in 
	// different ranges: [-2, +2], [-4, +4], [-8, +8], [-16, +16],
	// The hardware always return a value in the [-32768, +32767] range
	// which needs to be "scaled down" to the range it was configured for.
	// This member stores the positive half of the range the accelerometer
	// was configured to (for eg: 2, 4, 8 or 16g).
	double 		mAccelerometerHalfScaleRange;	
	
	// The same principle applies to the gyroscope with the
	// ranges in deg/sec: [-250, 250], [-500, 500], [-1000, 1000], [-2000, 2000]
	double 		mGyroscopeHalfScaleRange;	
};

MPU6050Device::MPU6050Device( const std::string& pihaIdentifier, uint8_t address )
	: Device(pihaIdentifier, "MPU6050"),
	  mMPU6050(NULL),
	  mAccelerometerHalfScaleRange(2),
	  mGyroscopeHalfScaleRange(250)
{
	mMPU6050 = new MPU6050(address);
	mMPU6050->initialize();
	
  if ( mMPU6050->testConnection() )
	{
    // Note: the initial setting of the encapsulated I2C device should happen here
    mMPU6050->setI2CMasterModeEnabled(0);     // !!!!!!!!!!!!!
    mMPU6050->setI2CBypassEnabled(1);         // !!!!!!!!!!!!!

 		
		mMPU6050->setFullScaleGyroRange(MPU6050_GYRO_FS_500);

std::cout<<"===> getRate:" << (int)mMPU6050->getRate() << std::endl; 
std::cout<<"===> getDLPFMode: " << (int)mMPU6050->getDLPFMode() << std::endl;

		mAccelerometerHalfScaleRange = getAccelerometerHalfScaleRange();
		Accelerometer* accelerometer = new Accelerometer("");
		addComponent( accelerometer );
		
		mGyroscopeHalfScaleRange = getGyroscopeHalfScaleRange();
		Gyroscope* gyroscope = new Gyroscope("");
		addComponent( gyroscope );
		
		TemperatureSensor* temperatureSensor = new TemperatureSensor("On-chip");
		addComponent( temperatureSensor );
	}
	else
	{
		printf("Failed to connect to MPU6050 at %d\n", address);		// !!!!
		delete mMPU6050;
		mMPU6050 = NULL;
	}
}

MPU6050Device::~MPU6050Device()
{
	delete mMPU6050;
	mMPU6050 = NULL;
}

	

//!!!!!!!!!!!!!
void splitString( const std::string& text, char delim, std::vector<std::string>& tokens ) 
{
    std::stringstream stream(text);
    std::string token;
    while ( std::getline(stream, token, delim) ) 
        tokens.push_back(token);
}

bool MPU6050Device::sendCommand( const std::string& command, std::string& result )	
{
	std::vector<std::string> tokens;
	splitString( command, ' ', tokens );
	std::string operation;
	std::string argument;
	if ( tokens.size()>=0 )
		operation = tokens[0];
	if ( tokens.size()>=1 )
		argument = tokens[1];

for ( std::size_t i=0; i<tokens.size(); ++i )
{
	std::cout << tokens[i] << "/";
}
std::cout << std::endl;
std::cout << "operation=" << operation << std::endl;
std::cout << "argument=" << argument << std::endl;
		
	if ( operation=="setRate" )
	{
		int argInt = atoi(argument.c_str() );
		mMPU6050->setRate(argInt);
		std::cout << "sesetRate(" << argInt << ")" << std::endl;
	}
	else if ( operation=="setDLPFMode" )
	{
		int argInt = atoi(argument.c_str() );
		mMPU6050->setDLPFMode(argInt);
		std::cout << "setDLPFMode(" << argInt << ")" << std::endl;
	}
	return true;
}

double MPU6050Device::getAccelerometerHalfScaleRange() const
{
	uint8_t scaleCode = mMPU6050->getFullScaleAccelRange();
	switch ( scaleCode )
	{
		case 0 : return 2.0;
		case 1 : return 4.0;
		case 2 : return 8.0;
		case 3 : return 16.0;
	}
	printf("getAccelerometerHalfScaleRange unsupported value returned\n");		// !!!!
	return 2.0;
}

double MPU6050Device::getGyroscopeHalfScaleRange() const
{
	uint8_t scaleCode = mMPU6050->getFullScaleGyroRange();
	switch ( scaleCode )
	{
		case 0 : return 250.0;
		case 1 : return 500.0;
		case 2 : return 1000.0;
		case 3 : return 2000.0;
	}
	printf("getGyroscopeHalfScaleRange unsupported value returned\n");		// !!!!
	return 250.0;
}

void MPU6050Device::update()
{
	if (!mMPU6050)
		return;

	// http://www.i2cdevlib.com/forums/topic/4-understanding-raw-values-of-accelerometer-and-gyrometer/
	// http://invensense.com/mems/gyro/documents/RM-MPU-6000A.pdf
	Accelerometer* accelerometer = static_cast<Accelerometer*>(getComponents()[0]);
	if ( accelerometer->isStarted() )
	{
		int16_t ax=0;
		int16_t ay=0;
		int16_t az=0;
		mMPU6050->getAcceleration(&ax, &ay, &az);
		double axf = static_cast<double>(ax) * mAccelerometerHalfScaleRange / 32768.0;		// Note: need to check this code!!
		double ayf = static_cast<double>(ay) * mAccelerometerHalfScaleRange / 32768.0;
		double azf = static_cast<double>(az) * mAccelerometerHalfScaleRange / 32768.0;
		accelerometer->setAccelerationInGs( Vector3d(axf, ayf, azf) );		/// A convertir comme il faut!	
	}
	else
	{
		accelerometer->setAccelerationInGs( Vector3d() );
	}
	
	Gyroscope* gyroscope = static_cast<Gyroscope*>(getComponents()[1]);
	if ( gyroscope->isStarted() )
	{
		int16_t gx=0;
		int16_t gy=0;
		int16_t	gz=0;
		mMPU6050->getRotation(&gx, &gy, &gz);
		double gxf = static_cast<double>(gx) * mGyroscopeHalfScaleRange / 32768.0;		// Note: need to check this code!!
		double gyf = static_cast<double>(gy) * mGyroscopeHalfScaleRange / 32768.0;
		double gzf = static_cast<double>(gz) * mGyroscopeHalfScaleRange / 32768.0;
		gyroscope->setAngularRateInDegPerSec( Vector3d(gxf, gyf, gzf) );		
	}
	else
	{
		gyroscope->setAngularRateInDegPerSec( Vector3d() );
	}
	
	TemperatureSensor* temperatureSensor = static_cast<TemperatureSensor*>(getComponents()[2]);
	if ( temperatureSensor->isStarted() )
	{
		int16_t temperature = mMPU6050->getTemperature();
		float temperatureInC = static_cast<float>(temperature)/340.f + 36.53f;
		temperatureSensor->setTemperatureInDegreeCelsius( temperatureInC );
	}
	else
	{
		temperatureSensor->setTemperatureInDegreeCelsius( 0.f );
	}
}

/*
	ProviderI2CDevLib
*/	
ProviderI2CDevLib::ProviderI2CDevLib( const std::string& i2cFilename, const I2cDeviceIdentifiers& i2cDeviceIdentifiers )
	: mDevices()  // not great, redundant with base class
{
	// Set the I2C bus device filename. 
	// Unfortunately, at the moment, this is a static property of the i2cdev lib
	// which means only one bus can be used at a time (the Raspberry Pi's got 2)
	I2Cdev::setI2CFilename( i2cFilename.c_str() );
	
	for ( std::size_t i=0; i<i2cDeviceIdentifiers.size(); ++i )
	{
		I2cDeviceType deviceType = i2cDeviceIdentifiers[i].getType();				
		uint8_t deviceAddress = i2cDeviceIdentifiers[i].getAddress();	

		// Construct the identifier in the sense of Piha. It is a composition of the device filename/path plus the I2C address
		std::stringstream stream;
		stream << i2cFilename << " (@" << static_cast<int>(deviceAddress) << ")";
		std::string pihaIdentifier = stream.str();
		
		Device* device = NULL;
		switch ( deviceType )
		{
			case kBMP085: device = new BMP085Device(pihaIdentifier, deviceAddress);
				break;
			case kMPU6050: device = new MPU6050Device(pihaIdentifier, deviceAddress);
				break;
			case kHMC5883L: device = new HMC5883LDevice(pihaIdentifier, deviceAddress);
			  break;
			case kMS561101BA: device = new MS561101BADevice(pihaIdentifier, deviceAddress);
				break;
			default:// Unsupported device type!
				break;
		}
		if ( device /* && device->isValid()*/ )
		  mDevices.push_back(device);
	}
}

ProviderI2CDevLib::~ProviderI2CDevLib()
{
}

void ProviderI2CDevLib::update()
{
	if ( getDevices().empty() )		// First device manager update: we add the devices (which notifies client code)
	{
		for ( std::size_t i=0; i<mDevices.size(); ++i )
			addDevice( mDevices[i] );
	}
	else
	{
		for ( std::size_t i=0; i<mDevices.size(); ++i )		// Later updates, we simply update each device
			mDevices[i]->update();
	}
}

} 
