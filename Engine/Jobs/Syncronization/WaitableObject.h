#pragma once

#include <Windows.h>

namespace Engine
{

	class WaitableObject
	{
	public:
		typedef DWORD wait_t;

		static const wait_t			WaitInfinite = INFINITE;

		WaitableObject() :
			m_Handle(INVALID_HANDLE_VALUE)
		{}
		virtual ~WaitableObject() {}

		HANDLE GetHandle(void) { return m_Handle; }

	protected:
		HANDLE					m_Handle;

	private:
		WaitableObject(const WaitableObject& i_other) = delete;
		WaitableObject& operator=(const WaitableObject& i_other) = delete;
	};

} // namespace Engine
