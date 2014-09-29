#ifndef __ASYNC_CALL_BACK_H__
#define __ASYNC_CALL_BACK_H__
#include <Windows.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>
#include "basic.hpp"
namespace bluetooch
{
	struct async_callback_base_t: 
	boost::noncopyable,
	OVERLAPPED 
	{
		async_callback_base_t()
		{
			memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));
		}
		virtual ~async_callback_base_t() {}
		virtual void invoke(const boost::system::error_code &error, int32 size) = 0;
		virtual void deallocate() = 0; 
	};
	template <typename HandlerT>
	struct win_async_callback_t
		: async_callback_base_t
	{
		typedef win_async_callback_t<HandlerT> this_t;
		HandlerT handler_;
		explicit win_async_callback_t(HandlerT& callback)
		:handler_(boost::move(callback))
		{
		}
		virtual ~win_async_callback_t()
		{
		}
		virtual void invoke(const boost::system::error_code &error, int32 size)
		{
			handler_(error, size);
		}
	}; 
	template <typename OverlappedT>
	void callback(OverlappedT *overlapped, int32 size, const boost::system::error_code &error)
	{
		async_callback_base_ptr p(static_cast<async_callback_base_t *>(overlapped));
		p->invoke(error, size);
	} 
	typedef boost::shared_ptr<async_callback_base_t> async_callback_ptr;
}
#endif