#include "SharedJobQueue.h"

#include <assert.h>

#include "..\Console\ConsolePrint.h"

namespace Engine
{
	namespace JobSystem
	{
		SharedJobQueue::SharedJobQueue() :
			m_ShutdownRequested(false)
		{
			InitializeConditionVariable(&m_WakeAndCheck);
			InitializeCriticalSection(&m_QueueAccess);
		}

		bool SharedJobQueue::Add(struct JobData* i_pJob)
		{
			assert(i_pJob);
			bool bAdded = false;

			EnterCriticalSection(&m_QueueAccess);
			if (m_ShutdownRequested == false)
			{
				m_Jobs.push(i_pJob);
				bAdded = true;
			}
			LeaveCriticalSection(&m_QueueAccess);

			if (bAdded)
				WakeConditionVariable(&m_WakeAndCheck);

			return bAdded;
		}

		bool SharedJobQueue::HasJobs() const
		{
			EnterCriticalSection(&m_QueueAccess);
			bool bFinished = m_Jobs.empty() && !m_bJobRunning;
			LeaveCriticalSection(&m_QueueAccess);

			return !bFinished;
		}

		void SharedJobQueue::JobFinished()
		{
			EnterCriticalSection(&m_QueueAccess);
			m_bJobRunning = false;
			LeaveCriticalSection(&m_QueueAccess);
		}

		struct JobData* SharedJobQueue::GetWhenAvailable()
		{
			EnterCriticalSection(&m_QueueAccess);

			if (m_Jobs.empty() && (m_ShutdownRequested == false))
			{
				BOOL result = SleepConditionVariableCS(&m_WakeAndCheck, &m_QueueAccess, INFINITE);
				assert(result != 0);

				if (m_ShutdownRequested == true)
				{
					LeaveCriticalSection(&m_QueueAccess);
					return nullptr;
				}
			}

			struct JobData* pJob = nullptr;

			if (!m_Jobs.empty())
			{
				pJob = m_Jobs.front();
				m_Jobs.pop();
			}

			m_bJobRunning = true;

			LeaveCriticalSection(&m_QueueAccess);
			return pJob;
		}

	} // namespace JobSystem
} // namespace Engine
