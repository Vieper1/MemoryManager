#pragma once

#include "Mutex.h"

namespace Engine
{
	class ScopeLock
	{
	public:
		ScopeLock(Mutex& i_Mutex) :
			m_pMutex(&i_Mutex)
		{
			m_pMutex->Acquire();
		}
		~ScopeLock()
		{
			m_pMutex->Release();
		}

	private:
		ScopeLock(const ScopeLock&) = delete;
		ScopeLock& operator=(const ScopeLock&) = delete;

		Mutex * m_pMutex;
	};

} // namespace Engine