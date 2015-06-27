
#if _WIN32
	#define WIN32_LEAN_AND_MEAN 
	#define NOMINMAX 
	#include <windows.h>
	#include "PihaWindowsDevice.h"
#else
	#include <unistd.h>
	#include "PihaLinuxDevice.h"
#endif

#include <stdio.h>
#include <cstring>

#include "PihaTime.h"
#include "PihaAccelerometer.h"
#include "PihaGyroscope.h"
#include "PihaMagnetometer.h"
#include "PihaBarometer.h"
#include "PihaTemperatureSensor.h"

class Test
{
public:
	static void sleep(unsigned int milliseconds)
	{
	#if _WIN32
	  ::Sleep(milliseconds);
	#else
	  usleep(milliseconds*1000);
	#endif
	}
};

const char* indentString = "  ";

std::string makeIndent( int indentLevel )
{
	std::string ret = "";
	for ( int i=0; i<indentLevel; ++i )
		ret += indentString;
	return ret;
}

void printDevice( Piha::Device* device, int indentLevel )
{
	std::string indent0 = makeIndent( indentLevel );
	std::string indent1 = makeIndent( indentLevel+1 );

	std::string parentDeviceName = "<none>";
	if ( device->getParentDevice() )
		parentDeviceName = "'" + device->getParentDevice()->getName() + "'";
	printf("%sdevice:'%s' parentDevice:%s\n", indent0.c_str(), device->getName().c_str(), parentDeviceName.c_str() );
	for ( std::size_t i=0; i<device->getComponents().size(); ++i )
	{
		Piha::Component* component = device->getComponents()[i];
		//printf("%sComponent '%s'\n", indent1.c_str(), component->getName().c_str() );
		printf("%s%s\n", indent1.c_str(), component->toString().c_str() );
	}

	for ( std::size_t i=0; i<device->getChildDevices().size(); ++i )
	{
		Piha::Device* childDevice = device->getChildDevices()[i];
		printDevice( childDevice, indentLevel+1 );
	}
}

// Use NULL as name filter to accept Components with any names,
// Use kNumTypes for the type filter to accept any types of component
void findComponents( Piha::Device* device, const char* nameFilter, Piha::Component::Type typeFilter, bool recursively, Piha::Components& foundComponents ) 
{
	const Piha::Components& components = device->getComponents();
	for ( Piha::Components::const_iterator itr=components.begin(); itr!=components.end(); itr++ )
	{
		bool nameAccepted = false;
		Piha::Component* component = *itr;
		if ( nameFilter!=0 )
		{
			const char* name = component->getName().c_str();
			if ( strcmp(name, nameFilter)==0 )
				nameAccepted = true;
		}
		else
		{
			nameAccepted = true;
		}
	
		bool typeAccepted = false;
		if ( typeFilter!=Piha::Component::kNumTypes )
		{
			if ( typeFilter==component->getType() )
				typeAccepted = true;
		}
		else 
		{
			typeAccepted = true;
		}
		
		if ( nameAccepted && typeAccepted )
			foundComponents.push_back( component );
	}
	if ( recursively )
	{
		const Piha::Devices& childDevices = device->getChildDevices();
		for ( Piha::Devices::const_iterator itr=childDevices.begin(); itr!=childDevices.end(); ++itr )
		{
			Piha::Device* childDevice = *itr;
			findComponents( childDevice, nameFilter, typeFilter, recursively, foundComponents );
		}
	}
}

class MyDeviceListener : public Piha::Device::Listener
{
public:
  virtual void onChildDeviceAdded(Piha::Device* /*device*/, Piha::Device* childDevice)
  {
    // Automatically start some of the components on the Device
    Piha::Components components = childDevice->getComponents();
    for (std::size_t i = 0; i < components.size(); ++i)
    {
      Piha::Component* component = components[i];
      switch (component->getType())
      {
        case Piha::Component::kAccelerometer:     static_cast<Piha::Accelerometer*>(component)->start();     
          break;          
        case Piha::Component::kGyroscope:         static_cast<Piha::Gyroscope*>(component)->start();          
          break;
        case Piha::Component::kMagnetometer:      static_cast<Piha::Magnetometer*>(component)->start();     
          break;
        case Piha::Component::kBarometer:         static_cast<Piha::Barometer*>(component)->start();            
          break;
        case Piha::Component::kTemperatureSensor: static_cast<Piha::TemperatureSensor*>(component)->start(); 
          break;
      }
    }
  }
};

void testPiha()
{
	Piha::Device* device = NULL;
#ifdef _WIN32
	device = new Piha::WindowsDevice( true );
#else
	device = new Piha::LinuxDevice();
#endif

  MyDeviceListener listener;    
  device->addChildDeviceChangeListener(&listener);   // Only things happening to Devices directly under the main Device will be notified
  device->addComponentChangeListener(&listener);   // Only things happening to Devices directly under the main Device will be notified

	int sleepTimeInMs = 500;
	int durationInMs = 10000;
	int c=0;
	do
	{
		device->update();
		printDevice( device, 0 );
		Test::sleep(sleepTimeInMs);
		printf("\n");
		c++;
	}
	while ( c<durationInMs/sleepTimeInMs );

printf("DELETING DEVICE!\n");
	device->removeComponentChangeListener(&listener);
	device->removeChildDeviceChangeListener(&listener);

	delete device;
	device = NULL;
printf("DEVICE DELETED!\n");
}

int main(int /*argc*/, char** /*argv*/ )
{
printf("STARTING!\n");
	testPiha();
printf("DONE!\n");
	return 0;
}

