#pragma once

#include <Windows.h>

#include "INamed.h"

namespace Engine
{
	namespace JobSystem
	{
		class IJob
#ifdef _DEBUG
			: public INamed
#endif
		{
		public:
			IJob(const char* i_pName) :
				INamed(i_pName)
			{}

			virtual					~IJob() { }
			virtual void			Run() = 0;

			static void				Shutdown();
			static bool				ShutdownRequested();
		private:
			static CRITICAL_SECTION		m_Shutdown;
			static bool					m_bShutdown;
		};

		inline bool IJob::ShutdownRequested()
		{
			return m_bShutdown;
		}

	} // namespace JobSystem
} // namespace Engine