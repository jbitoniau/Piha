#pragma once

#include <string>
#include <vector>
#include "PihaComponent.h"

#ifdef PIHA_XINPUT_SUPPORT
#pragma message("XINPUT")
#endif

namespace Piha
{

class Device;
typedef std::vector<Device*> Devices;

/*
	Device

	The Device object is the corner stone of Piha. It represents a physical device connected 
	to the computer or an existing device. It exposes a set of Components that represents
	actual physical components providing a functionality (button, axis, camera, accelerometer, etc...).

	A Device can appear and disappear from the device tree. The components of a device however are 
	created with the Device and don't change over time. 
*/
class Device
{
public:
	Device( const std::string& identifier, const std::string& name );
	virtual ~Device();
	
	Device*				getParentDevice() const		{ return mParentDevice; }

	// Return an identifier string that (hopefully) uniquely locates the device in the tree.
	// Storing the identifier might be useful for an application to retrieve the device in 
	// a later session (if it's still part of the device tree). There are few things to understand
	// however.
	// 
	// A device might have a serial number (for eg: Phidget device) that identifies it uniquely 
	// amongst all the other products of the manufacturer (two manufacturers might accidentally 
	// have the same numbering scheme). This is similar to a MAC address or Bluetooth address. 
	// A serial number can be used as an idenfitier for the Piha Device, in which case it refers 
	// to the exact same physical device (devices of the same model will be seen as different 
	// devices)
	//
	// However most devices lack such number, the identifier might therefore be more related to 
	// the way the device is connected to the computer. This can be something based on 
	// the USB address (for eg: DirectShow device), or a port number (for eg: Pololu Maestro, 
	// or XBox 360 controller index). The identifier in this case cannot distinguish between
	// two physical instances connected at different moment on the same port. 
	const std::string&	getIdentifier() const		{ return mIdentifier; }

	// The name represents the device product/model name (for eg "XBox 360 controller", 
	// "Logitech Webcam XYZ", etc...). It can include the manufacturer name or not. It is not 
	// a unique tree-wide identifier for the device as  there can multiple devices from the model 
	// connected to the computer.
	const std::string&	getName() const				{ return mName; }
	const Components&	getComponents() const		{ return mComponents; }
	const Devices&		getChildDevices() const		{ return mChildDevices; }
	
	virtual bool		sendCommand( const std::string& /*command*/, std::string& /*result*/ )	{ return false; }

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void	onChildDeviceAdded( Device* /*device*/, Device* /*childDevice*/ ) {}
		virtual void	onChildDeviceRemoving( Device* /*device*/, Device* /*childDevice*/ ) {}
		virtual void	onDeviceComponentChanged( Device* /*device*/, Component* /*component*/ ) {}
	};

	void				addChildDeviceChangeListener( Listener* listener );
	bool				removeChildDeviceChangeListener( Listener* listener );
	void				addComponentChangeListener( Listener* listener );
	bool				removeComponentChangeListener( Listener* listener );
	
	virtual void		update() = 0;

protected:
	void				setParentDevice( Device* parentDevice );

	void				addComponent( Component* component );
	void				deleteComponents();
	friend class Component;
	void				notifyComponentChanged( Component* component );

	void				addChildDevice( Device* device );
	void				removeChildDevice( Device* device );	// This does not delete the device
	void				deleteChildDevices();

	typedef std::vector<Listener*> Listeners; 
	const Listeners&	getChildDeviceChangeListeners() const	{ return mChildDeviceChangeListeners; }
	const Listeners&	getComponentChangeListeners() const		{ return mComponentChangeListeners; }

private:
	Device*				mParentDevice;
	std::string			mIdentifier;
	std::string			mName;
	Components			mComponents;
	Devices				mChildDevices;
	Listeners			mChildDeviceChangeListeners;
	Listeners			mComponentChangeListeners;
};

}