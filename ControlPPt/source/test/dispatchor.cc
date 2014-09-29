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
	dispatchor::dispatchor(size_t num_threads)
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
			impl_->bind(handle);
		}
	}
	// 向完成端口投递请求
	template<typename HandlerT>
	void dispatchor::post(HandlerT& handle)
	{
		if (impl_)
		{
			async_callback_ptr async_callback(new win_async_callback_t);
			impl_->post_impl();
		}
	}
	class dispatchor::impl
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
				thread_pool_.push_back(boost::move(boost::thread(boost::bind(&impl::run ,this))));
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
			 BOOST_FOREACH(boost::thread& t, thread_pool_)
			 {
				 ::QueueUserAPC(ApcQueue, t.native_handle(), 0); 
			 }
			 BOOST_FOREACH(boost::thread&t, thread_pool_)
			 {
				 t.join();
			 }
			 thread_pool_.clear(); 
		 }
		 bool post_impl(const async_callback_ptr &val)
		 {
			 if (!iocp_.post_status(0, 0, static_cast<OVERLAPPED *>(val.get())))
				 throw ("iocp_.PostStatus");

			 return true;
		 }
	private:
		void run()
		{

		}
		iocp_handle iocp_;
		std::vector<boost::thread> thread_pool_; 
	}
}

