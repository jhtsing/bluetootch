#ifndef __IOCP_IMPL_H__
#define __IOCP_IMPL_H__
#include "iocp.h"
#include <vector>
#include <thread>
#include <mutex>
#include "async_callback.h" 
namespace service
{
	class iocp_impl
	{
	public:
		iocp_impl(int max_threads);
		~iocp_impl();
		void stop();
		void post(HandlerType handler);
		void bind(HANDLE handle);
	private:
		bool start(int thread_count);
		void run();
		iocp iocp_;
		std::mutex threads_mutex_;
		std::vector<std::thread> threads_;
	};
}
#endif 