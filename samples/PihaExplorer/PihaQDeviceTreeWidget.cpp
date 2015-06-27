#include "PihaQDeviceTreeWidget.h"

#include <assert.h>
#include "PihaQDeviceTreeItem.h"
#include "PihaQComponentTreeItem.h"

namespace Piha
{

QDeviceTreeWidget::QDeviceTreeWidget( QWidget* parent )
	: QTreeWidget(parent), 
	  mDevice(NULL)
{
	QStringList headerLabels;
	headerLabels.push_back( "Item" );
	headerLabels.push_back( "Identifier" );
	headerLabels.push_back( "Name" );
	headerLabels.push_back( "State" );
	headerLabels.push_back( "Editor" );
	setHeaderLabels( headerLabels );

	bool ret = connect(	this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), 
						this, SLOT( onItemClicked( QTreeWidgetItem*, int)));
	assert( ret );
}

QDeviceTreeWidget::~QDeviceTreeWidget()
{
}

void QDeviceTreeWidget::setDevice( Device* device )
{
	if ( mDevice )
		clear();

	mDevice = device;
	/*QDeviceTreeItem* deviceItem =*/ new QDeviceTreeItem( this, mDevice );
	

	/*assert( !mRootDevice );
	if ( mRootDevice )
		return;

	mRootDevice = _Device;

	mRootItem = new QDeviceTreeItem(this, QDeviceTreeItem::DEVICE, _Device );
	QDeviceTreeItem::initTree( mRootItem );
	*/		
}

/*void QDeviceTreeWidget::update()
{
	if ( mRootDevice )
		mRootDevice->update();
	
	// update device timings
	if ( mRootItem )
		updateDeviceUpdateDuration( mRootItem );

	mMyWidget->update();
}*/

void QDeviceTreeWidget::onItemClicked( QTreeWidgetItem* item, int column )
{
	if ( dynamic_cast<QDeviceTreeItem*>(item) )
		static_cast<QDeviceTreeItem*>(item)->onItemClicked(column);
	else if ( dynamic_cast<QComponentTreeItem*>(item) )
		static_cast<QComponentTreeItem*>(item)->onItemClicked(column);
	
/*	QDeviceTreeItem* deviceItem = static_cast<QDeviceTreeItem*>( item );
	if ( deviceItem->getType()==QDeviceTreeItem::COMPONENT )
	{
		if ( column==1 )
		{
			Jois::Component* component = deviceItem->getComponent();
			Jois::RumbleMotor* rumbleMotor = Jois::CastAsRumbleMotor( component );
			if ( rumbleMotor )
			{
				rumbleMotor->setActive( !rumbleMotor->isActive() );
			}
		}
	}*/
}

}
