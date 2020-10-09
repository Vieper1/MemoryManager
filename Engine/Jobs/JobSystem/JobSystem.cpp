#include "JobSystem.h"

#include <Windows.h>
#include <map>

#include "..\Console\ConsolePrint.h"
#include "..\HashedString\HashedString.h"

#include "JobRunner.h"
#include "SharedJobQueue.h"

namespace Engine
{
	namespace JobSystem
	{
		struct JobRunnerData
		{
			HANDLE				m_ThreadHandle;
			DWORD				m_ThreadID;
			JobRunnerInput		m_Input;
		};

		struct JobQueueData
		{
			SharedJobQueue		m_SharedQueue;
			std::vector<JobRunnerData*>	m_Runners;
		};

		bool bShutdownRequested = false;
		std::map<HashedString, JobQueueData*>	Queues;

		void RequestShutdown()
		{
			DEBUG_PRINT("Job System: Shutting down.\n");

			bShutdownRequested = true;

			std::vector<HANDLE>	AllThreads;

			std::map<HashedString, JobQueueData*>::iterator iter = Queues.begin();
			while (iter != Queues.end())
			{

				if (iter->second)
				{
					const size_t count = iter->second->m_Runners.size();
					for (size_t i = 0; i < count; i++)
					{
						JobRunnerData* pRunner = iter->second->m_Runners[i];
						if (pRunner && pRunner->m_ThreadHandle != NULL)
							AllThreads.push_back(pRunner->m_ThreadHandle);
					}
				}
				++iter;
			}

			DEBUG_PRINT("Job System: Waiting for Queue runner threads to shut down.\n");

			DWORD result = WaitForMultipleObjects(static_cast<DWORD>(AllThreads.size()), &AllThreads[0], TRUE, INFINITE);
			assert(result == WAIT_OBJECT_0);

			iter = Queues.begin();
			while (iter != Queues.end())
			{
				if (iter->second)
				{
					const size_t count = iter->second->m_Runners.size();
					for (size_t i = 0; i < count; i++)
					{
						JobRunnerData* pRunner = iter->second->m_Runners[i];
						if (pRunner)
							delete pRunner;
					}

					delete iter->second;
				}

				++iter;
			}

			Queues.clear();

		}

		bool ShutdownRequested()
		{
			return bShutdownRequested;
		}

		void AddRunner(JobQueueData& i_QueueData)
		{
			size_t runners = i_QueueData.m_Runners.size();

			JobRunnerData* pNewRunner = new JobRunnerData;

			pNewRunner->m_Input.i_pQueue = &i_QueueData.m_SharedQueue;
#ifdef _DEBUG
			const size_t	sizeThreadName = 32;
			char			ThreadName[sizeThreadName];
			sprintf_s(ThreadName, sizeThreadName, "%s %d", i_QueueData.m_SharedQueue.GetName(), int(runners + 1));
			pNewRunner->m_Input.m_ThreadName = ThreadName;
#endif

			pNewRunner->m_ThreadHandle = CreateThread(NULL, 0, JobRunner, &pNewRunner->m_Input, CREATE_SUSPENDED, &pNewRunner->m_ThreadID);
			assert(pNewRunner->m_ThreadHandle != NULL);

			i_QueueData.m_Runners.push_back(pNewRunner);
			ResumeThread(pNewRunner->m_ThreadHandle);
		}

		void AddRunner(const HashedString& i_QueueName)
		{
			std::map<HashedString, JobQueueData*>::iterator existing = Queues.find(i_QueueName);
			assert(existing != Queues.end());
			assert(existing->second);

			AddRunner(*existing->second);
		}

		void CreateQueue(const char* i_pName, unsigned int i_numRunners)
		{
			assert(i_pName);

			HashedString	HashedName(i_pName);

			assert(Queues.find(HashedName) == Queues.end());

			JobQueueData* pNewJobQueueData = new JobQueueData;
			pNewJobQueueData->m_SharedQueue.SetName(i_pName);

			Queues[HashedName] = pNewJobQueueData;

			for (unsigned int i = 0; i < i_numRunners; i++)
				AddRunner(*pNewJobQueueData);
		}

		bool HasJobs(const HashedString& i_QueueName)
		{
			std::map<HashedString, JobQueueData*>::iterator existing = Queues.find(i_QueueName);
			if (existing != Queues.end())
			{
				if (existing->second)
					return existing->second->m_SharedQueue.HasJobs();
			}

			return false;
		}

		void RunJob(const std::string& i_JobName, std::function<void()> i_JobFunction, const HashedString& i_QueueName)
		{
			std::map<HashedString, JobQueueData*>::iterator existing = Queues.find(i_QueueName);
			assert(existing != Queues.end());
			assert(existing->second);

			DEBUG_PRINT("Job System: Adding Job to Queue \"%s\".\n", existing->second->m_SharedQueue.GetName());
			existing->second->m_SharedQueue.Add(new JobData(i_JobName, i_JobFunction));
		}

	} // namespace JobSystem
} // namespace Engine

