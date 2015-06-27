#pragma once

namespace Piha
{

template<typename T>
struct Vector3
{
public:
	Vector3() 
		: mX(0), mY(0), mZ(0)
	{
	}
	
	Vector3( T x, T y, T z ) 
		: mX(x), mY(y), mZ(z)
	{
	}
	
	Vector3( const Vector3& other )
	{
		mX = other.mX;
		mY = other.mY;
		mZ = other.mZ;
	}

	Vector3& operator=( const Vector3& other )
	{
		if ( this==&other )
			return *this;
		mX = other.mX;
		mY = other.mY;
		mZ = other.mZ;
		return *this;
	}

	bool operator==( const Vector3& other ) const
	{
		return	( mX==other.mX && 
				  mY==other.mY && 
				  mZ==other.mZ );
	}

	bool operator!=( const Vector3& other ) const
	{
		return	!( *this==other );
	}

	T& x()				{ return mX; }
	const T& x() const	{ return mX; }
		
	T& y()				{ return mY; }
	const T& y() const	{ return mY; }
	
	T& z()				{ return mZ; }
	const T& z() const	{ return mZ; }
	
private:
	T mX;
	T mY;
	T mZ;	
};

typedef Vector3<double>	Vector3d;
typedef Vector3<float>	Vector3f;

}