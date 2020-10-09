#include <assert.h>
#include <windows.h>

#include "Mutex.h"

namespace Engine
{

	Mutex::Mutex(bool i_bTakeOwnership, const char* i_pName)
	{
		m_Handle = CreateMutex(NULL, (BOOL)i_bTakeOwnership, i_pName);
		assert(m_Handle != NULL);
	}

	Mutex::~Mutex()
	{
		BOOL result = CloseHandle(m_Handle);
		assert(result == TRUE);
	}

	bool Mutex::TryAcquire(void)
	{
		DWORD result = WaitForSingleObject(m_Handle, 0);

		return result == WAIT_OBJECT_0;
	}

	void Mutex::Acquire(void)
	{
		DWORD result = WaitForSingleObject(m_Handle, INFINITE);
		assert(result == WAIT_OBJECT_0);

	}

	bool Mutex::Acquire(wait_t i_WaitMilliseconds)
	{
		DWORD result = WaitForSingleObject(m_Handle, i_WaitMilliseconds);
		assert(((i_WaitMilliseconds == WaitInfinite) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));

		return result == WAIT_OBJECT_0;
	}

	void Mutex::Release(void)
	{
		BOOL result = ReleaseMutex(m_Handle);
		assert(result == TRUE);
	}

} // namespace Engine