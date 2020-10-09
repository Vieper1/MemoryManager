#pragma once

#include <Windows.h>

namespace Engine
{
	class SingleWriterMutipleReader
	{
	public:
		SingleWriterMutipleReader()
		{
			InitializeSRWLock(&m_RWLock);
		}

		void ReadLock()
		{
			AcquireSRWLockShared(&m_RWLock);
		}
		bool TryReadLock()
		{
			return TryAcquireSRWLockShared(&m_RWLock) == TRUE;
		}
		void ReleaseReadLock()
		{
			ReleaseSRWLockShared(&m_RWLock);
		}

		void WriteLock()
		{
			AcquireSRWLockExclusive(&m_RWLock);
		}
		bool TryWriteLock()
		{
			return TryAcquireSRWLockExclusive(&m_RWLock) == TRUE;
		}
		void ReleaseWriteLock()
		{
			ReleaseSRWLockShared(&m_RWLock);
		}

	private:
		SingleWriterMutipleReader(const SingleWriterMutipleReader&) = delete;
		SingleWriterMutipleReader& operator=(const SingleWriterMutipleReader&) = delete;
		
		SRWLOCK			m_RWLock;
	};

} // namespace Engine

