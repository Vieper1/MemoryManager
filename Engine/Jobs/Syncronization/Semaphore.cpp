#include "Semaphore.h"

#include <assert.h>
#include <Windows.h>

namespace Engine
{

	Semaphore::Semaphore(unsigned int i_initCount, unsigned int i_maxCount, const char* i_pName)
	{
		assert(i_initCount <= i_maxCount);

		m_Handle = CreateSemaphore(NULL, i_initCount, i_maxCount, i_pName);
		assert(m_Handle != NULL);
	}

	Semaphore::~Semaphore()
	{
		BOOL result = CloseHandle(m_Handle);
		assert(result == TRUE);
	}

	// should 
	void Semaphore::Increment(unsigned int i_count)
	{
		BOOL result = ReleaseSemaphore(m_Handle, i_count, NULL);
		assert(result == TRUE);
	}

	void Semaphore::Decrement(void)
	{
		DWORD result = WaitForSingleObject(m_Handle, INFINITE);
		assert(result == WAIT_OBJECT_0);
	}

	bool Semaphore::Decrement(wait_t i_WaitMilliseconds)
	{
		DWORD result = WaitForSingleObject(m_Handle, i_WaitMilliseconds);
		assert(((i_WaitMilliseconds == WaitInfinite) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));

		return result == WAIT_OBJECT_0;
	}

} // namespace Engine
