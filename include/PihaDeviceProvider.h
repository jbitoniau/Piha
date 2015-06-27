#pragma once

#include "PihaDevice.h"

namespace Piha
{

/*
	DeviceProvider

	A DeviceProvider is an object capable of constructing Device instances from an 
	underlying system API. 

	The root Device of a device tree, the one that represents the computer itself, 
	typically uses a series of DeviceProviders to enumerate the devices (camera,
	game controller, etc...) that are connected to the system and instantiate the 
	corresponding Devices. The life and death of a Device in the device tree is 
	then the responsibility of the DeviceProvider which constructed it in the first 
	place.
	
	The benefit of the DeviceProvider approach is that it allows the user to select
	at runtime which APIs Piha Devices will be constructed from.
*/
class DeviceProvider
{
public:
	DeviceProvider();

	// The base class implementation of the destructor takes care of deleting any Devices
	// that have been created and added (via addDevice())
	virtual ~DeviceProvider();

	const Devices&	getDevices() const;

	virtual void	update() = 0;

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void onDeviceAdded( DeviceProvider* /*deviceProvider*/, Device* /*device*/ ) {}
		virtual void onDeviceRemoving( DeviceProvider* /*deviceProvider*/, Device* /*device*/ ) {}
	};

	void			addListener( Listener* listener );
	bool			removeListener( Listener* listener );

protected:
	void			addDevice( Device* device );
	void			deleteDevice( Device* device );
	void			deleteDevices();

	typedef			std::vector<Listener*> Listeners; 
	const Listeners& getListeners() const { return mListeners; }

private:
	Devices			mDevices;
	Listeners		mListeners;
};

}