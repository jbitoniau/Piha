#include "PihaGPSReceiver.h"

#include <sstream>

namespace Piha
{

/*
	GPSReceiver::Measure
*/
GPSReceiver::Measure::Measure()
	: mLatitudeInDecDeg(0.0),
	  mLongitudeInDecDeg(0.0),
	  mAltitudeInM(0.0)
{
}

GPSReceiver::Measure::Measure( double latitudeInDecDeg, double longitudeInDecDeg, double altitudeInM )
	: mLatitudeInDecDeg(latitudeInDecDeg),
	  mLongitudeInDecDeg(longitudeInDecDeg),
	  mAltitudeInM(altitudeInM)
{
}

bool GPSReceiver::Measure::operator==( const Measure& other ) const
{
	return	( mLatitudeInDecDeg==other.mLatitudeInDecDeg &&
			  mLongitudeInDecDeg==other.mLongitudeInDecDeg &&
			  mAltitudeInM==other.mAltitudeInM );
}

bool GPSReceiver::Measure::operator!=( const Measure& other ) const
{
	return	!( *this==other );
}

std::string GPSReceiver::Measure::toString() const
{
	std::stringstream stream;
	stream << std::fixed;
	stream.precision(3);
	stream << " measure: latitudeInDecDeg:" << getLatitudeInDecDeg() << " longitudeInDecDeg:" << getLongitudeInDecDeg() << " altitudeInM:" << getAltitudeInM();
	return stream.str();
}

/*
	GPSReceiver
*/
GPSReceiver::GPSReceiver( const std::string& name )
	: Component(kGPSReceiver, name),
	  mIsStarted(false),
	  mMeasure()
{
}

bool GPSReceiver::start()
{
	mIsStarted = true;
	return true;
}

void GPSReceiver::stop()
{
	mIsStarted = false;
}

void GPSReceiver::setMeasure( const Measure& measure )
{
	if ( mMeasure==measure )
		return;
	mMeasure=measure;
	notifyComponentChanged();
}

std::string	GPSReceiver::toString() const
{
	std::stringstream stream;
	const Measure& measure = getMeasure();
	stream << Component::toString() << measure.toString();
	return stream.str();
}

}

