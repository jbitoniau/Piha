#pragma once

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )	
#endif
#include <QTreeWidgetItem>
#ifdef _MSC_VER
	#pragma warning( pop )
#endif

#include "PihaDevice.h"

namespace Piha
{

class QDeviceTreeWidget;
class QComponentTreeItem;

class QDeviceTreeItem :	public QTreeWidgetItem, 
						public Device::Listener
{
public: 
	QDeviceTreeItem( QDeviceTreeWidget* parent, Device* device );
	QDeviceTreeItem( QDeviceTreeItem* parent, Device* device );
	virtual ~QDeviceTreeItem();

	Device*			getDevice() const	{ return mDevice; }
	//void			update();

	virtual	void	onItemClicked( int column );

protected:
	void			init();
	virtual void	onChildDeviceAdded( Device* device, Device* childDevice );
	virtual void	onChildDeviceRemoving( Device* device, Device* childDevice );
	virtual void	onDeviceComponentChanged( Device* device, Component* component );

	QComponentTreeItem* findChildComponentTreeItem( const Component* component ) const;
	QDeviceTreeItem*	findChildDeviceTreeItem( const Device* device ) const;

private:
	Device*			mDevice;
};

}
