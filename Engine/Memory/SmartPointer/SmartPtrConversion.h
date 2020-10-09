#include "SmartPtr.h"

class SmartPtrConversion
{
public:
	template <typename DestT, typename SrcT>
	static SmartPtr<DestT> SmartPtrCast(const SmartPtr<SrcT> & src)
	{
		SmartPtr<DestT> destSmartPtr;
		destSmartPtr.ptr = static_cast<DestT*>(src.ptr);
		destSmartPtr.refCount = src.refCount;
		++destSmartPtr.refCount->SmartPtrCount;
		return destSmartPtr;
	}
};
