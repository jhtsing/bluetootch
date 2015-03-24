#include "include\iocp_impl.h"

namespace service
{
	void __stdcall ApcQueue(ULONG_PTR)
	{}
	void handle_callback(OVERLAPPED *overlapped, std::uint32_t size, const std::error_code &error)
	{
		async_callback_base_ptr p(static_cast<async_callback_base_t *>(overlapped));
		p->invoke(error, size);
	}
	iocp_impl::iocp_impl(int max_threads)
	{
		if (!iocp_.create(max_threads))
			printf("iocp create!\n");
		if (!start(max_threads))
			printf("start create!\n");
	}
	iocp_impl::~iocp_impl()
	{
		try
		{
			stop();
			iocp_.close();
		}
		catch (std::exception& ec)
		{
		}
	}
	bool iocp_impl::start(int thread_count)
	{
		try
		{
			std::unique_lock <std::mutex> lck(threads_mutex_);
			threads_.reserve(thread_count);
			for (int i = 0; i < thread_count; i++)
			{
				//std::thread thread_handle(&iocp_impl::run,this);
				//threads_.push_back(thread_handle);
				threads_.push_back(std::thread([this]
				{
					run();
				}));
			}
		}
		catch (std::exception& ec)
		{
			return false;
		}
		return true;
	}
	void iocp_impl::stop()
	{
		for (int i = 0; i < threads_.size(); i++)
		{
			::QueueUserAPC(ApcQueue, threads_[i].native_handle(), 0);
		}
		for (int i = 0; i < threads_.size(); i++)
		{
			threads_[i].join();
		}
		std::unique_lock <std::mutex> lock(threads_mutex_);
		threads_.clear();
	}
	void iocp_impl::post(HandlerType handler)
	{
		async_callback_base_ptr ptr = make_callback_base_ptr(handler);
		iocp_.post(static_cast<OVERLAPPED *>(ptr.get()));
		ptr.release();
	}
	void iocp_impl::bind(HANDLE handle)
	{
		iocp_.bind(handle, 0);
	}
	void iocp_impl::run()
	{
		OVERLAPPED_ENTRY entrys[64] = { 0 };
		DWORD ret_number = 0;
		while (true)
		{
			::SetLastError(0);
			iocp_.get_status_ex(entrys, 64, ret_number);
			DWORD error = GetLastError();
			if (error == WAIT_IO_COMPLETION)
			{
				break;
			}
			try
			{
				for (int i = 0; i != ret_number; ++i)
				{
					handle_callback(
						entrys[i].lpOverlapped,
						entrys[i].dwNumberOfBytesTransferred,
						std::make_error_code((std::errc)entrys[i].Internal));
				}
			}
			catch (std::exception& ec)
			{
				break;
			}
		}
	}
}