#include "PihaQDeviceTreeItem.h"

#include <assert.h>
#include "PihaQDeviceTreeWidget.h"
#include "PihaQComponentTreeItem.h"

namespace Piha
{

QDeviceTreeItem::QDeviceTreeItem( QDeviceTreeWidget* parent, Device* device )
	:	QTreeWidgetItem( parent ),
		mDevice(device)
{
	init();
}
	
QDeviceTreeItem::QDeviceTreeItem( QDeviceTreeItem* parent, Device* device )
	:	QTreeWidgetItem( parent ),
		mDevice(device)
{
	init();
}

QDeviceTreeItem::~QDeviceTreeItem()
{
	getDevice()->removeComponentChangeListener(this);
	getDevice()->removeChildDeviceChangeListener(this);
}

void QDeviceTreeItem::init()
{
	getDevice()->addChildDeviceChangeListener(this);
	getDevice()->addComponentChangeListener(this);

	setText( 0, "Device" );
	setText( 1, QString::fromUtf8( getDevice()->getIdentifier().c_str() ) );
	setText( 2, QString::fromUtf8( getDevice()->getName().c_str() ) );
	setExpanded( true );

	const Devices& childDevices = getDevice()->getChildDevices();
	for ( std::size_t i=0; i<childDevices.size(); ++i )
	{
		/*QDeviceTreeItem* deviceItem =*/ new QDeviceTreeItem( this, childDevices[i] );
	}

	const Components& components = getDevice()->getComponents();
	for ( std::size_t i=0; i<components.size(); ++i )
	{
		/*QComponentTreeItem* componentItem =*/ new QComponentTreeItem( this, components[i] );
	}
}

void QDeviceTreeItem::onItemClicked( int /*column*/ )
{
}

void QDeviceTreeItem::onChildDeviceAdded( Device* device, Device* childDevice )
{
	assert( device==getDevice() );
	/*QDeviceTreeItem* childDeviceItem =*/ new QDeviceTreeItem( this, childDevice );
}

void QDeviceTreeItem::onChildDeviceRemoving( Device* device, Device* childDevice )
{
	assert( device==getDevice() );
	QDeviceTreeItem* childDeviceItem = findChildDeviceTreeItem( childDevice );
	assert( childDeviceItem );
	removeChild( childDeviceItem );	
}

void QDeviceTreeItem::onDeviceComponentChanged( Device* device, Component* component )
{
	assert( device==getDevice() );
	QComponentTreeItem* componentItem = findChildComponentTreeItem( component );
	assert( componentItem );
	componentItem->updateContents();
}

QComponentTreeItem* QDeviceTreeItem::findChildComponentTreeItem( const Component* component ) const
{
	for ( int i=0; i<childCount(); ++i )
	{
		QComponentTreeItem* item = dynamic_cast<QComponentTreeItem*>( child(i) );
		if ( item && item->getComponent()==component )
			return item;
	}
	return NULL;
}

QDeviceTreeItem* QDeviceTreeItem::findChildDeviceTreeItem( const Device* device ) const
{
	for ( int i=0; i<childCount(); ++i )
	{
		QDeviceTreeItem* item = dynamic_cast<QDeviceTreeItem*>( child(i) );
		if ( item && item->getDevice()==device )
			return item;
	}
	return NULL;
}

}