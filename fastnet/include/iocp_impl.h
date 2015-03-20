#ifndef __IOCP_IMPL_H__
#define __IOCP_IMPL_H__
#include "iocp.h"
#include <vector>
#include <thread>
#include <mutex>
#include "async_callback.h" 
class iocp_impl
{
public:
	iocp_impl(int max_threads);
	~iocp_impl();
	bool start(int thread_count);
	void stop();
	void post(HandlerType handler);
	void bind(HANDLE handle);
private:
	void run();
	iocp iocp_;
	std::mutex threads_mutex_;
	std::vector<std::thread> threads_;
};

#endif 