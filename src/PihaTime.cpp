#include "PihaTime.h"

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <cstddef>		// For NULL
	//#include <sys/time.h>
	#include <time.h>
#endif

#include <assert.h>

namespace Piha
{
	
#ifdef _WIN32
/*
	TimeInternals (Windows platform)

	On Windows, there are many time functions available.
	- the standard ftime function (sys/timeb.h). Millisecond resolution but 15ms accuracy
	- GetSystemTimeAsFileTime(). Millisecond resolution but 15ms accuracy
	- GetTickCount(). Millisecond resolution but up to 50 ms accuracy (to double check)
	The only high resolution/accuracy one I found is the Performance counter but it seems not 
	very reliable on multi-core machines see  http:www.virtualdub.org/blog/pivot/entry.php?id=106
	Anyway, I'm using it for now. I think this can be improved: 
	http:msdn.microsoft.com/en-us/magazine/cc163996.aspx
*/
class TimeInternals
{
public:
	TimeInternals();

	unsigned long long int	getTickFrequency() const		{ return mTickFrequency; }
	unsigned long long int	getTickCount() const;
	unsigned int			getTimeAsMilliseconds() const;
	
	static TimeInternals&	getInstance()					{ return mTimeInternals; }
	
private:
	static unsigned long long int internalGetTickFrequency();
	static unsigned long long int internalGetTickCount();
	
	unsigned long long int	mTickFrequency;
	unsigned long long int	mInitialTickCount;
	
	static TimeInternals	mTimeInternals;
};

/*
	TimeInternals (Windows platform)
*/
TimeInternals TimeInternals::mTimeInternals;

TimeInternals::TimeInternals()
{
	mTickFrequency = internalGetTickFrequency();	// Cache the tick frequency as it doesn't change during the application lifetime
	mInitialTickCount = internalGetTickCount();
}

unsigned long long int TimeInternals::getTickCount() const
{ 
	unsigned long long int tickCount = internalGetTickCount();
	tickCount -= mInitialTickCount;
	return tickCount;
}

unsigned int TimeInternals::getTimeAsMilliseconds() const 
{
	unsigned int time = static_cast<unsigned int>( (getTickCount()*1000) / getTickFrequency() );
	return time;
}

unsigned long long int TimeInternals::internalGetTickFrequency()
{ 
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency.QuadPart; 
}

unsigned long long int TimeInternals::internalGetTickCount()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	return counter.QuadPart;
}

#else

/*
	TimeInternals (non-Windows platform)

	This implementation relies on the monotonic version of clock_gettime() function.
	Changing the system date/time won't affect this time.

	See http://code-factor.blogspot.fr/2009/11/monotonic-timers.html
*/
class TimeInternals
{
public:
	TimeInternals();

	unsigned int			getTimeAsMilliseconds() const;
	
	static TimeInternals&	getInstance()					{ return mTimeInternals; }
	
private:
	struct timespec			mInitialTime;

	static TimeInternals	mTimeInternals;
};

/*
	TimeInternals (non-Windows platform)
*/
TimeInternals TimeInternals::mTimeInternals;

TimeInternals::TimeInternals()
{
	int ret = clock_gettime( CLOCK_MONOTONIC, &mInitialTime );
	assert( ret==0 );
}

unsigned int TimeInternals::getTimeAsMilliseconds() const 
{
	struct timespec theTime;
	int ret = clock_gettime( CLOCK_MONOTONIC, &theTime );
	assert( ret==0 );

	time_t numSeconds = theTime.tv_sec;
	long numNanoSeconds = theTime.tv_nsec;	// 1ns = 10^-9 seconds
	
	unsigned int milliseconds = static_cast<unsigned int>(numSeconds - mInitialTime.tv_sec) * 1000 + 
								static_cast<unsigned int>(numNanoSeconds / 1000000) - 
								static_cast<unsigned int>(mInitialTime.tv_nsec / 1000000);
	return milliseconds;
}

/*

	TimeInternals (non-Windows platform)

	This is obsolete!
	Here we use the gettimeofday function (http://man7.org/linux/man-pages/man2/gettimeofday.2.html) 
	This can lead to dramatic results if the system time is changed behind the application's back!
	See
		http://tldp.org/HOWTO/Clock-2.html
		http://linuxcommand.org/man_pages/adjtimex8.html
		http://stackoverflow.com/questions/588307/c-obtaining-milliseconds-time-on-linux-clock-doesnt-seem-to-work-properl
		http://linux.die.net/man/3/clock_gettime

*/
/*
class TimeInternals
{
public:
	TimeInternals();

	unsigned int			getTimeAsMilliseconds() const;
	
	static TimeInternals&	getInstance()					{ return mTimeInternals; }
	
private:
	timeval					mInitialTime;
	
	static TimeInternals	mTimeInternals;
};
*/
/*
	TimeInternals (non-Windows platform)
*/
/*
TimeInternals TimeInternals::mTimeInternals;

TimeInternals::TimeInternals()
{
	gettimeofday( &mInitialTime, NULL );
}

unsigned int TimeInternals::getTimeAsMilliseconds() const 
{
	struct timeval theTime;
	gettimeofday(&theTime, NULL);
	
	time_t numSeconds = theTime.tv_sec;
	suseconds_t numMicroSeconds = theTime.tv_usec;
	unsigned int milliseconds = static_cast<unsigned int>(numSeconds - mInitialTime.tv_sec) * 1000 + 
								static_cast<unsigned int>(numMicroSeconds) / 1000 - 
								static_cast<unsigned int>(mInitialTime.tv_usec) / 1000;
	return milliseconds;
}
*/
#endif

/*
	Time
*/
unsigned int Time::getTimeAsMilliseconds()
{
	unsigned int milliseconds = TimeInternals::getInstance().getTimeAsMilliseconds();
	return milliseconds;
}

}
