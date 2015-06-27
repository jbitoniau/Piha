#pragma once

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning ( disable : 4127 )
	#pragma warning ( disable : 4231 )
	#pragma warning ( disable : 4251 )
	#pragma warning ( disable : 4800 )	
#endif
#include <QtCore/QTimer>
#ifdef _MSC_VER
	#pragma warning( pop )
#endif

#include "PihaDevice.h"

namespace Piha
{

class QDeviceUpdater : QObject
{
	Q_OBJECT

public:
	QDeviceUpdater( Device* device, int intervalInMs, QObject* parent );
	virtual		~QDeviceUpdater();
	void		start();
	void		stop();

protected slots:
	void		timerTimeout();

private:
	Device*		mDevice;
	QTimer*		mTimer;
};

}