#pragma once 

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )	
#endif
#include <QTreeWidget>
#include <QPainter>
#ifdef _MSC_VER
	#pragma warning( pop )
#endif

#include "PihaDevice.h"

namespace Piha
{

class QDeviceTreeWidget : public QTreeWidget
{
	Q_OBJECT

public: 
	QDeviceTreeWidget( QWidget* parent );
	virtual ~QDeviceTreeWidget();

	void				setDevice( Device* device );
	Device*				getDevice() const			{ return mDevice; }
	
private slots:
	//void				update();
	void				onItemClicked( QTreeWidgetItem* item, int column );

private:
	Device*				mDevice;
};

}
