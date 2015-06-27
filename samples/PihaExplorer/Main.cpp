#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )	
#endif
#include <QApplication>
#include <QTreeWidget>
#ifdef _MSC_VER
	#pragma warning( pop )
#endif

#include "PihaQDeviceTreeWidget.h"
#include "PihaQDeviceUpdater.h"

#ifdef _WIN32
	#include "PihaWindowsDevice.h"
#else
	#include "PihaLinuxDevice.h"
#endif

using namespace Piha;
 
int main( int argc, char** argv )
{
	QApplication app(argc, argv);
	Device* rootDevice = NULL;
#ifdef _WIN32
	rootDevice = new Piha::WindowsDevice(false);
#else
	rootDevice = new Piha::LinuxDevice();
#endif
	QDeviceTreeWidget* tree = new QDeviceTreeWidget(NULL);
	
	tree->setDevice( rootDevice );

	QDeviceUpdater* updater = new QDeviceUpdater( rootDevice, 20, tree );
	updater->start();

	rootDevice->update(); // So the tree gets populated and we can resize the columns just below...
	tree->expandAll();
	for ( int i=0; i<tree->columnCount(); ++i )
		tree->resizeColumnToContents(i);
	tree->setColumnWidth(1, 100);		// Identifier column can have quite large content
	

	tree->resize(600, 480);
	tree->show();
/*

	QGraphPlotterWidget* w = new QGraphPlotterWidget(NULL);
	w->resize(600, 480);
	w->show();*/
	int ret = app.exec();
	
	delete tree;		// This deletes the DeviceUpdater
	tree = NULL;
	
	delete rootDevice;
	rootDevice = NULL;

	return ret;
}
