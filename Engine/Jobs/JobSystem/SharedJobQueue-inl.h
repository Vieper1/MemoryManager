namespace Engine
{
	namespace JobSystem
	{
		inline void SharedJobQueue::Shutdown()
		{
			EnterCriticalSection(&m_QueueAccess);
			m_ShutdownRequested = true;
			LeaveCriticalSection(&m_QueueAccess);

			WakeAllConditionVariable(&m_WakeAndCheck);
		}

		inline bool SharedJobQueue::ShutdownRequested() const
		{
			return m_ShutdownRequested;
		}

	} // namespace JobSystem
} // namespace Engine