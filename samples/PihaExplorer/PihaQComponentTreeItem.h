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

#include "PihaComponent.h"

namespace Piha
{

class QDeviceTreeItem;

class QComponentTreeItem :	public QTreeWidgetItem/*, 
							public Device::Listener*/
{
public: 
	QComponentTreeItem( QDeviceTreeItem* parent, Component* component );
	virtual ~QComponentTreeItem();

	Component*		getComponent() const	{ return mComponent; }
	
	virtual void	updateContents();

	virtual	void	onItemClicked( int column );

private:
	static void		capitalizeFirstLetter( std::string& text );

private:
	Component*		mComponent;
};

}