#include "timer_impl.hpp"
#include <Windows.h>
#include <assert.h>
#include <iostream>

	waitable_timer_t::waitable_timer_t(long period, long due, bool manualReset, const wchar_t *timerName)
		: timer_(NULL)
		, period_(period)
		, due_(due)
	{
		timer_ = ::CreateWaitableTimerW(NULL, manualReset ? TRUE : FALSE, timerName);
	}
	waitable_timer_t::~waitable_timer_t()
	{
		if( timer_ != NULL )
		{
			::CloseHandle(timer_);
			timer_ = NULL;
		}
	}
	void waitable_timer_t::set_timer(long period, long long delay)
	{
		assert(timer_ != NULL);

		LARGE_INTEGER dueTime = {0};
		dueTime.QuadPart = -(delay * 10000);

		period_ = period;
		due_ = delay;

		if( !::SetWaitableTimer(timer_, &dueTime, period, NULL, NULL, TRUE) )
			std::cout<<"SetWaitableTimer failed"<<std::endl;
	}

	void waitable_timer_t::cancel()
	{
		assert(timer_ != NULL);
		if( !::CancelWaitableTimer(timer_) )
			std::cout << "CancelWaitableTimer failed" << std::endl;
	}

	void waitable_timer_t::sync_wait() const
	{
		assert(timer_ != NULL);

		DWORD res = ::WaitForSingleObject(timer_, period_);
		if( res == WAIT_FAILED )
			std::cout << "WaitForSingleObject failed" << std::endl;
	}

	void waitable_timer_t::async_wait()
	{
		assert(timer_ != NULL);

		set_timer(period_, due_);
	}