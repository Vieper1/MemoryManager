#pragma once

#include "RefCount.h"




template <class T>
class SmartPtr
{
	template <class T>
	friend class WeakPtr;
	friend class SmartPtrConversion;

	RefCount * refCount;
	T * ptr;

public:
	// Construct
	explicit SmartPtr(T * pointer = nullptr) : 
		refCount(new RefCount(1, 0)),
		ptr(pointer)
	{}

	// Copy
	SmartPtr(const SmartPtr & other) :
		refCount(other.refCount),
		ptr(other.ptr)
	{
		if (ptr && refCount)
			refCount->SmartPtrCount++;
	}
	
	// SmartPtr(const WeakPtr & other);

	// Move
	SmartPtr(const SmartPtr && other) noexcept :
		refCount(other.refCount),
		ptr(other.ptr)
	{
		++other.refCount->SmartPtrCount;
	}

	// Assign
	SmartPtr & operator = (const SmartPtr & other)
	{
		if (this->ptr == other.ptr)
			return *this;
		
		SafeRelease();
		ptr = other.ptr;
		refCount = other.refCount;
		refCount->SmartPtrCount++;
		return *this;
	}
	SmartPtr & operator = (const SmartPtr && other)
	{
		return *this;
	}
	SmartPtr & operator = (std::nullptr_t other)
	{
		SafeRelease();
		ptr = nullptr;
		refCount = nullptr;
		return *this;
	}

	// Destruct
	~SmartPtr()
	{
		SafeRelease();
		refCount = nullptr;
		ptr = nullptr;
	}

	int getSmartCount() const
	{
		return refCount->SmartPtrCount;
	}


	operator bool() const { return ptr ? true : false; }
	bool operator == (const T * t) { return (t == ptr) ? true : false; }
	bool operator == (const SmartPtr & other) { return (other.ptr == ptr) ? true : false; }
	T & operator * () { return *ptr; }
	T * operator -> () { return ptr; }

	
	
private:
	SmartPtr(T * i_ptr, RefCount * i_refCount) : 
		refCount(i_refCount),
		ptr(i_ptr)
	{
		if (ptr && refCount)
			refCount->SmartPtrCount++;
	}

	void SafeRelease() const
	{
		if (refCount)
		{
			refCount->SmartPtrCount--;
			if (refCount->SmartPtrCount <= 0)
			{
				delete ptr;
				delete refCount;
			}
		}
	}
};