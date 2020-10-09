#pragma once

#include "WaitableObject.h"

namespace Engine
{

	class Semaphore : public WaitableObject
	{
	public:
		Semaphore(unsigned int i_initCount, unsigned int i_maxCount, const char* i_pName = nullptr);
		~Semaphore();

		void Increment(unsigned int i_Count = 1);
		void Decrement();
		bool Decrement(wait_t i_WaitMilliseconds);
	private:
		Semaphore(const Semaphore&) = delete;
		Semaphore& operator=(const Semaphore&) = delete;
	};

} // namespace Engine

