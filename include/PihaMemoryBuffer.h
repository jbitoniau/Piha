#pragma once

namespace Piha
{

/*
	MemoryBuffer
	Represents a raw block of data in memory (aka a blob). 
*/
class MemoryBuffer
{
public:
	// Creates an empty MemoryBuffer 
	MemoryBuffer();
	
	// Creates a MemoryBuffer of the given size 
	MemoryBuffer( unsigned int sizeInBytes );

	// Creates a MemoryBuffer from existing data/bytes present in memory.
	// The MemoryBuffer references this data and does not own it. 
	// The data in memory must remain valid throughout the lifetime of the MemoryBuffer.
	// Deleting the MemoryBuffer will not delete the underlying data.
	MemoryBuffer( unsigned char* bytes, unsigned int sizeInBytes );
	
	// Creates a new MemoryBuffer as a copy of another one. The underlying bytes
	// are copied. This new MemoryBuffer is independent from the other one 
	// Deleting the MemoryBuffer will delete the copied bytes not the ones used originally.
	MemoryBuffer( const MemoryBuffer& other );

	~MemoryBuffer();	

	unsigned int			getSizeInBytes() const	{ return mSizeInBytes; }
	const unsigned char*	getBytes() const		{ return mBytes; }
	unsigned char*			getBytes()				{ return mBytes; }
	
	void					fill( char value );
	bool					copyFrom( const MemoryBuffer& other );
	bool					copyFrom( const unsigned char* bytes, unsigned int sizeInBytes );

private:
	MemoryBuffer& operator=( const MemoryBuffer& other );	// Not implemented on purpose

	unsigned char*			mBytes;
	unsigned int			mSizeInBytes;
	bool					mOwnsBytes;
};

}
