#include "PihaDeviceProvider.h"

#include <assert.h>
#include <algorithm>
#include "PihaDevice.h"

namespace Piha
{

DeviceProvider::DeviceProvider()
{
}

DeviceProvider::~DeviceProvider()
{
	deleteDevices();
}

void DeviceProvider::addDevice( Device* device )
{
	mDevices.push_back( device );
	for ( Listeners::iterator itrListener=mListeners.begin(); itrListener!=mListeners.end(); ++itrListener )
		(*itrListener)->onDeviceAdded( this, device );
}

void DeviceProvider::deleteDevice( Device* device )
{
	Devices::iterator itr = std::find( mDevices.begin(), mDevices.end(), device );
	if ( itr==mDevices.end() )
		return;
	for ( Listeners::iterator itrListener=mListeners.begin(); itrListener!=mListeners.end(); ++itrListener )
		(*itrListener)->onDeviceRemoving( this, device );
	mDevices.erase( itr );
	delete device;
}

void DeviceProvider::deleteDevices()
{	
	Devices	devices = mDevices;		// The copy is on purpose here
	for ( Devices::iterator itr=devices.begin(); itr!=devices.end(); itr++ )
	{
		Device* device = *itr;
		deleteDevice( device );
	}
	assert( mDevices.empty() );
}

const Devices& DeviceProvider::getDevices() const
{
	return mDevices;
}

void DeviceProvider::addListener( Listener* listener )
{
	assert(listener);
	mListeners.push_back(listener);
}

bool DeviceProvider::removeListener( Listener* listener )
{
	Listeners::iterator itr = std::find( mListeners.begin(), mListeners.end(), listener );
	if ( itr==mListeners.end() )
		return false;
	mListeners.erase( itr );
	return true;
}

}