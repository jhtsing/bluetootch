#include "StdAfx.h"
#include <vector>
#include <boost/thread.hpp>
#include "dispatchor.hpp"


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
	template<typename HandlerT, typename AllocatorT = std::allocator<char> >
	void dispatchor::post(HandlerT &&, const AllocatorT &allocator = AllocatorT())
	{
		if (impl_)
		{
			impl_->
		}
	}
	class dispatchor::impl
	{
	public:
		 impl(int num_thread):
		 {
			if (!iocp_.create(num_thread))
			{
				 throw ("create iocp failed");
			}
			thread_pool_.reserve(num_thread);
		    for (int i = 0 ; i < num_thread ; i++)
			{
				thread_pool_.push_back(boost::move(boost::bind(&impl::run ,this)));
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
			 BOOST_FOREACH(boost::thread& t ,threads_)
			 {
				 ::QueueUserAPC(ApcQueue, t.native_handle(), 0); 
			 }
			 BOOST_FOREACH(boost::thread&t , threads_)
			 {
				 t.join();
			 }
			 thread_pool_.clear(); 
		 }
		 void post();
	private:
		void run()
		{

		}
		iocp_handle iocp_;
		std::vector<boost::thread> thread_pool_; 
	}
}

