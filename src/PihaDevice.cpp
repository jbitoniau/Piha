#include "PihaDevice.h"

#include <assert.h>
#include <algorithm>


/*
	Note:
	- investigate Device recursive destruction and notifications. It should go depth-first properly 
*/
namespace Piha
{

Device::Device( const std::string& identifier, const std::string& name )
	: mParentDevice(NULL),
	  mIdentifier(identifier),
	  mName(name),
	  mComponents(),
	  mChildDevices(),
	  mChildDeviceChangeListeners(),
	  mComponentChangeListeners()
{
}

Device::~Device()
{
	deleteComponents();
	deleteChildDevices();
}

void Device::setParentDevice( Device* parentDevice ) 
{
	// Make sure we're either we're setting the parent device 
	// when the device doesn't have anyor we're removing the parent
	assert( (!getParentDevice() && parentDevice) ||  
		    (getParentDevice() && !parentDevice) ); 
	mParentDevice = parentDevice;
}

void Device::addComponent( Component* component )
{
	assert( component );
	component->setParentDevice( this, mComponents.size() );
	mComponents.push_back( component );	
}

void Device::deleteComponents()
{
	for ( Components::iterator itr=mComponents.begin(); itr!=mComponents.end(); ++itr )
		delete *itr;
	mComponents.clear();
}

void Device::notifyComponentChanged( Component* component )
{
	for ( Listeners::iterator itrListener=mComponentChangeListeners.begin(); itrListener!=mComponentChangeListeners.end(); ++itrListener )
		(*itrListener)->onDeviceComponentChanged( this, component );
}

void Device::addChildDevice( Device* device )
{
	device->setParentDevice( this );
	mChildDevices.push_back( device );
	for ( Listeners::iterator itrListener=mChildDeviceChangeListeners.begin(); itrListener!=mChildDeviceChangeListeners.end(); ++itrListener )
		(*itrListener)->onChildDeviceAdded( this, device );
}

void Device::removeChildDevice( Device* device )
{
	Devices::iterator itr = std::find( mChildDevices.begin(), mChildDevices.end(), device );
	if ( itr==mChildDevices.end() )
		return;
	for ( Listeners::iterator itrListener=mChildDeviceChangeListeners.begin(); itrListener!=mChildDeviceChangeListeners.end(); ++itrListener )
		(*itrListener)->onChildDeviceRemoving( this, device );
	mChildDevices.erase( itr );
	device->setParentDevice( NULL );
}

void Device::deleteChildDevices()
{	
	Devices	devices = mChildDevices;		// The copy is on purpose here
	for ( Devices::iterator itr=devices.begin(); itr!=devices.end(); itr++ )
	{
		Device* device = *itr;
		removeChildDevice( device );
		delete device;					
	}
	assert( mChildDevices.empty() );
}
	
void Device::addChildDeviceChangeListener( Listener* listener )
{
	assert(listener);
	mChildDeviceChangeListeners.push_back(listener);
}

bool Device::removeChildDeviceChangeListener( Listener* listener )
{
	Listeners::iterator itr = std::find( mChildDeviceChangeListeners.begin(), mChildDeviceChangeListeners.end(), listener );
	if ( itr==mChildDeviceChangeListeners.end() )
		return false;
	mChildDeviceChangeListeners.erase( itr );
	return true;
}
	
void Device::addComponentChangeListener( Listener* listener )
{
	assert(listener);
	mComponentChangeListeners.push_back(listener);
}

bool Device::removeComponentChangeListener( Listener* listener )
{
	Listeners::iterator itr = std::find( mComponentChangeListeners.begin(), mComponentChangeListeners.end(), listener );
	if ( itr==mComponentChangeListeners.end() )
		return false;
	mComponentChangeListeners.erase( itr );
	return true;
}

}