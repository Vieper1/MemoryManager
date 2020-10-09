#include "JobRunner.h"

#include <assert.h>

#include "JobSystem.h"
#include "../Console/ConsolePrint.h"

namespace Engine
{
	namespace JobSystem
	{

		DWORD WINAPI JobRunner(void* i_pThreadInput)
		{
			assert(i_pThreadInput);
			assert(((JobRunnerInput*)i_pThreadInput)->i_pQueue);

			JobRunnerInput* pInput = reinterpret_cast<JobRunnerInput*>(i_pThreadInput);
			SharedJobQueue* pQueue = reinterpret_cast<SharedJobQueue*>(pInput->i_pQueue);

			DEBUG_PRINT("JobRunner \"%s\": Starting Queue \"%s\".\n", pInput->m_ThreadName.c_str(), pQueue->GetName() ? pQueue->GetName() : "Unknown");

			bool bDone = false;

			do
			{
				DEBUG_PRINT("JobRunner \"%s\": Acquiring Job.\n", pInput->m_ThreadName.c_str());
				struct JobData* pJob = pQueue->GetWhenAvailable();
				if (pJob)
				{
					DEBUG_PRINT("JobRunner \"%s\": Starting Job \"%s\" on Processor %d.\n", pInput->m_ThreadName.c_str(), pJob->Name.c_str(), GetCurrentProcessorNumber());

					pJob->Function();

					DEBUG_PRINT("JobRunner \"%s\": Finished Job \"%s\".\n", pInput->m_ThreadName.c_str(), pJob->Name.c_str());
					delete pJob;
					pQueue->JobFinished();
					bDone = ShutdownRequested();
				}
				else
					bDone = ShutdownRequested();

			} while (bDone == false);

#ifdef _DEBUG
			DEBUG_PRINT("JobRunner \"%s\": Shutting down.\n", pInput->m_ThreadName.c_str());
#endif
			return 0;
		}

	} // namespace JobSystem
} // namespace Engine
