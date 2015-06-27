#pragma once

#include "PihaComponent.h"
#include "PihaVector3.h"

namespace Piha
{

class GPSReceiver : public Component
{
public:
	GPSReceiver( const std::string& name );
	
	virtual bool	start();
	virtual void	stop();
	bool			isStarted() const					{ return mIsStarted; }

	class Measure
	{
	public:
		Measure();
		Measure( double latitudeInDecDeg, double longitudeInDecDeg, double altitudeInM );
		bool		operator==( const Measure& other ) const;
		bool		operator!=( const Measure& other ) const;
				
		double		getLatitudeInDecDeg() const			{ return mLatitudeInDecDeg; }
		double		getLongitudeInDecDeg() const		{ return mLongitudeInDecDeg; }
		double		getAltitudeInM() const				{ return mAltitudeInM; }

		std::string toString() const;

	private:
		double		mLatitudeInDecDeg;
		double		mLongitudeInDecDeg;
		double		mAltitudeInM;
	};
	const Measure&	getMeasure() const					{ return mMeasure; }
	void			setMeasure( const Measure& measure );
	
	virtual std::string	toString() const;

protected:
	bool			mIsStarted;
	Measure			mMeasure;
};

}