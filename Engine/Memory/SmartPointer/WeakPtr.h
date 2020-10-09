#pragma once

#include "SmartPtr.h"


template <class T>
class WeakPtr
{
	friend SmartPtr<T>;

	RefCount * refCount;
	T * ptr;

public:
	// Construct
	WeakPtr() :
		refCount(nullptr),
		ptr(nullptr)
	{}

	// Copy
	WeakPtr(const WeakPtr & other) :
		refCount(other.refCount),
		ptr(other.ptr)
	{
		if (ptr && refCount)
			refCount->WeakPtrCount++;
	}
	WeakPtr(const SmartPtr<T> & other) :
		refCount(other.refCount),
		ptr(other.ptr)
	{
		if (ptr && refCount)
			refCount->WeakPtrCount++;
	}

	// Move
	WeakPtr(const WeakPtr && other) noexcept :
		refCount(other.refCount),
		ptr(other.ptr)
	{
		++other.refCount->WeakPtrCount;
	}

	// Assign
	WeakPtr & operator = (const SmartPtr<T> & other)
	{
		if (ptr == other.ptr)
			return *this;

		SafeRelease();
		ptr = other.ptr;
		refCount = other.refCount;
		refCount->WeakPtrCount++;
		return *this;
	}
	WeakPtr & operator = (const WeakPtr & other)
	{
		if (ptr == other.ptr)
			return *this;

		SafeRelease();
		ptr = other.ptr;
		refCount = other.refCount;
		return *this;
	}
	WeakPtr & operator = (const WeakPtr && other)
	{
		//if (ptr == other.ptr)
		//	return *this;

		//SafeRelease();
		//ptr = other.ptr;
		//refCount = other.refCount;
		return *this;
	}
	WeakPtr & operator = (std::nullptr_t other)
	{
		SafeRelease();
		ptr = nullptr;
		refCount = nullptr;
		return *this;
	}

	// Destruct
	~WeakPtr()
	{ 
		if (ptr && refCount && refCount->SmartPtrCount > 0) SafeRelease();
	}




	operator bool() const { return ptr ? true : false; }
	bool operator == (const T * t) { return (t == ptr) ? true : false; }
	bool operator == (const WeakPtr & other) { return (other.ptr == ptr) ? true : false; }
	bool operator == (const SmartPtr<T> & other) { return (other.ptr == ptr) ? true : false; }


	bool acquire(SmartPtr<T> & ref)
	{
		if (ptr && refCount && refCount->SmartPtrCount > 0) 
		{
			SmartPtr<T> toReturn(ptr, refCount);
			ref = toReturn;
			
			return true;
		}
		return false;
	}


private:
	void SafeRelease() const
	{
		if (refCount)
		{
			refCount->WeakPtrCount--;
			if (refCount && refCount->SmartPtrCount <= 0)
			{
				delete ptr;
				delete refCount;
			}
		}
	}
};