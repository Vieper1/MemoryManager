#pragma once

#include <Windows.h>
#include <queue>

#include "INamed.h"

namespace Engine
{
	namespace JobSystem
	{
		struct JobData;

		class SharedJobQueue
#ifdef _DEBUG
			: public INamed
#endif
		{
		public:
			SharedJobQueue();

			bool Add(struct JobData* i_JobData);
			bool HasJobs() const;
			struct JobData* GetWhenAvailable();
			void JobFinished();
		private:
			SharedJobQueue(const SharedJobQueue&) = delete;
			SharedJobQueue& operator=(const SharedJobQueue&) = delete;

			std::queue<struct JobData*>	m_Jobs;
			CONDITION_VARIABLE m_WakeAndCheck;
			mutable CRITICAL_SECTION m_QueueAccess;

			bool m_bJobRunning;
			bool m_ShutdownRequested;
		};

	} // namespace JobSystem
} // namespace Engine