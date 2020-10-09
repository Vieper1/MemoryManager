#pragma once

#include "WaitableObject.h"

namespace Engine
{

	class Mutex : public WaitableObject
	{
	public:
		Mutex(bool i_bTakeOwnership = false, const char* i_pName = nullptr);
		~Mutex();

		bool TryAcquire();
		void Acquire();
		bool Acquire(wait_t i_WaitMilliseconds);
		void Release();

	private:
		Mutex(const Mutex&) = delete;
		Mutex& operator=(const Mutex&) = delete;
	};

} // namespace Engine

