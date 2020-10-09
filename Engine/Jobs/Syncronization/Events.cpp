#include "Events.h"

#include <assert.h>
#include <Windows.h>

namespace Engine
{

	ManualResetEvent::ManualResetEvent(bool i_bInitiallySignaled, const char* i_pName)
	{
		m_Handle = CreateEvent(NULL, TRUE, i_bInitiallySignaled, i_pName);
		assert(m_Handle != INVALID_HANDLE_VALUE);
	}

	ManualResetEvent::~ManualResetEvent()
	{
		CloseHandle(m_Handle);
	}

	void ManualResetEvent::Reset(void)
	{
		ResetEvent(m_Handle);
	}

	bool ManualResetEvent::Wait(wait_t i_WaitMilliseconds)
	{
		DWORD result = WaitForSingleObject(m_Handle, i_WaitMilliseconds);
		assert(((i_WaitMilliseconds == WaitInfinite) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));

		return result == WAIT_OBJECT_0;
	}

	void ManualResetEvent::Signal(void)
	{
		BOOL result = SetEvent(m_Handle);
		assert(result == TRUE);
	}

	AutoResetEvent::AutoResetEvent(bool i_bInitiallySignaled, const char* i_pName)
	{
		m_Handle = CreateEvent(NULL, FALSE, i_bInitiallySignaled, i_pName);
		assert(m_Handle != INVALID_HANDLE_VALUE);
	}

	AutoResetEvent::~AutoResetEvent()
	{
		CloseHandle(m_Handle);
	}

	bool AutoResetEvent::Wait(wait_t i_WaitMilliseconds)
	{
		DWORD result = WaitForSingleObject(m_Handle, i_WaitMilliseconds);
		assert(((i_WaitMilliseconds == WaitInfinite) && (result == WAIT_OBJECT_0)) || (result == WAIT_TIMEOUT));

		return result == WAIT_OBJECT_0;
	}

	void AutoResetEvent::Signal(void)
	{
		BOOL result = SetEvent(m_Handle);
		assert(result == TRUE);
	}
} // namespace Engine