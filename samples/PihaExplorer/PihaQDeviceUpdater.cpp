#include "PihaQDeviceUpdater.h"

#include <assert.h>

namespace Piha
{

QDeviceUpdater::QDeviceUpdater( Device* device, int intervalInMs, QObject* parent )
	:	QObject(parent),
		mDevice(device),
		mTimer(NULL)
{
	mTimer = new QTimer(this);
	mTimer->setInterval( intervalInMs );	
	bool ret = connect( mTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()) );
	assert( ret );
	start();
}

QDeviceUpdater::~QDeviceUpdater()
{
}

void QDeviceUpdater::start()
{
	mTimer->start();
}

void QDeviceUpdater::stop()
{
	mTimer->stop();
}

void QDeviceUpdater::timerTimeout()
{
	mDevice->update();
}

}