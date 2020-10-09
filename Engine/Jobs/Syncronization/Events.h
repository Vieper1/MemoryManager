#pragma once

#include <Windows.h>

#include "WaitableObject.h"

namespace Engine
{
	class Event : public WaitableObject
	{
	public:
		virtual bool Wait(wait_t i_WaitMilliseconds) = 0;
		virtual void Signal() = 0;

	};

	class ManualResetEvent : public Event
	{
	public:
		ManualResetEvent(bool i_bInitiallySignaled = false, const char* i_pName = nullptr);
		~ManualResetEvent();

		void Reset();

		bool Wait(wait_t i_WaitMilliseconds = INFINITE);
		void Signal();
	private:
		ManualResetEvent(const ManualResetEvent&) = delete;
		ManualResetEvent& operator=(const ManualResetEvent&) = delete;
	};

	class AutoResetEvent : public Event
	{
	public:
		AutoResetEvent(bool i_bInitiallySignaled = false, const char* i_pName = nullptr);
		~AutoResetEvent();

		bool Wait(wait_t i_WaitMilliseconds = INFINITE);
		void Signal();
	private:
		AutoResetEvent(const AutoResetEvent&) = delete;
		AutoResetEvent& operator=(const AutoResetEvent&) = delete;
	};

} // namespace Engine