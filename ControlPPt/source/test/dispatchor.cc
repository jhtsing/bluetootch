#include "StdAfx.h"
#include <vector>
#include <Windows.h>
#include <boost/thread.hpp>
#include "dispatchor.hpp"
#include "iocp.hpp"
#include "call_back_type.h"
#include <boost/foreach.hpp>

namespace bluetooch
{
	// for thread exit,GetQueuedCompletionStatus get IO_COMPLETE_STATUS,
	void __stdcall ApcQueue(ULONG_PTR)
	{
	} 
	struct dispatchor::impl
	{
	public:
		impl(int num_thread)
		{
			if (!iocp_.create(num_thread))
			{
				throw ("create iocp failed");
			}
			thread_pool_.reserve(num_thread);
			for (int i = 0 ; i < num_thread ; i++)
			{
				boost::scoped_ptr<boost::thread> new_thread(new boost::thread(boost::move(boost::thread(boost::bind(&impl::run ,this)))));
				thread_pool_.push_back(new_thread.get());
			}
		}
		void bind(HANDLE handle)
		{
			if( !iocp_.associate_device(handle, 0) )
			{
				throw ("iocp_.associate_device"); 
			}
		}
		void stop()
		{
			// 先停止所有的线程
			BOOST_FOREACH(boost::thread* t, thread_pool_)
			{
				::QueueUserAPC(ApcQueue, t->native_handle(), 0); 
			}
			BOOST_FOREACH(boost::thread* t, thread_pool_)
			{
				t->join();
			}
			thread_pool_.clear(); 
		}
		template <typename HandlerT>
		bool post_impl(const boost::shared_ptr<async_callback_base_t> &val)
		{
			if (!iocp_.post_status(0, 0, static_cast<OVERLAPPED *>(val.get())))
				throw ("iocp_.PostStatus");
			return true;
		}
	private:
		void run()
		{

			OVERLAPPED* ol_overlapped;
			ULONG_PTR complete_key;
			DWORD read_bytes;
			while(true)
			{
				::SetLastError(0);
				bool suc = iocp_.get_status(&complete_key,&read_bytes ,&ol_overlapped);
				int err = ::GetLastError(); 
				if( err == WAIT_IO_COMPLETION )
				{
					break;
				}
				if (ol_overlapped==NULL)
				{
					break;
				}
				try
				{
					callback_t(ol_overlapped , read_bytes, boost::system::error_code());
				}
				catch (std::exception& ec)
				{
					//err handle
				}
			}
		}
		iocp_handle iocp_;
		std::vector<boost::thread*> thread_pool_; 
	};
	dispatchor::dispatchor(size_t num_threads):
	impl_(new impl(num_threads))
	{
	}
	dispatchor::~dispatchor(void)
	{
	}
	// 绑定端口 
	void dispatchor::bind(void *handle)
	{
		if (impl_)
		{
			impl_->bind((HANDLE)handle);
		}
	}
	// 向完成端口投递请求
	template<typename HandlerT>
	void dispatchor::post(HandlerT& handle)
	{
		if (impl_)
		{
			boost::shared_ptr<async_callback_base_t<HandlerT>>   async_callback = make_callback(handle);
			impl_->post_impl(async_callback);
		}
	}
}

