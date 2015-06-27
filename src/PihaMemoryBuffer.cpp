#include "PihaMemoryBuffer.h"

#include <stddef.h>		// For NULL
#include <memory.h>

namespace Piha
{

MemoryBuffer::MemoryBuffer()
	: mBytes(NULL),
	  mSizeInBytes(0),
	  mOwnsBytes(true)
{
}

MemoryBuffer::MemoryBuffer( unsigned int sizeInBytes )
	: mBytes(NULL),
	  mSizeInBytes(sizeInBytes),
	  mOwnsBytes(true)
{
	mBytes = new unsigned char[mSizeInBytes];
	fill(0);
}

MemoryBuffer::MemoryBuffer( unsigned char* bytes, unsigned int sizeInBytes )
	: mBytes(bytes),
	  mSizeInBytes(sizeInBytes),
	  mOwnsBytes(false)
{
}
	
MemoryBuffer::MemoryBuffer( const MemoryBuffer& other )
	: mBytes(NULL),
	  mSizeInBytes( other.getSizeInBytes() ),
	  mOwnsBytes(true)
{
	mBytes = new unsigned char[mSizeInBytes];
	memcpy( mBytes, other.getBytes(), other.getSizeInBytes() );
}

MemoryBuffer::~MemoryBuffer()
{
	if ( mOwnsBytes )
		delete[] mBytes;
	mBytes = NULL;
	mSizeInBytes = 0;
}

void MemoryBuffer::fill( char value )
{
	memset( mBytes, value, getSizeInBytes() );
}

bool MemoryBuffer::copyFrom( const MemoryBuffer& other )
{
	if ( other.getSizeInBytes()!=getSizeInBytes() )
		return false;
	memcpy( mBytes, other.getBytes(), getSizeInBytes() );
	return true;
}

bool MemoryBuffer::copyFrom( const unsigned char* bytes, unsigned int sizeInBytes )
{
	unsigned int n = sizeInBytes;
	if ( n>getSizeInBytes() )
		n = getSizeInBytes();
	memcpy( mBytes, bytes, n );
	return true;
}

}