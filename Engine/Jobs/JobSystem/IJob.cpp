#include "IJob.h"

#include <Windows.h>

namespace Engine
{
	namespace JobSystem
	{
		CRITICAL_SECTION	IJob::m_Shutdown;
		bool				IJob::m_bShutdown = false;

		void IJob::Shutdown()
		{
			static bool bInited = false;
			if (bInited == false)
			{
				InitializeCriticalSection(&m_Shutdown);
				bInited = true;
			}

			EnterCriticalSection(&m_Shutdown);
			m_bShutdown = true;
			LeaveCriticalSection(&m_Shutdown);
		}

	} // namespace JobSystem
} // namespace Engine