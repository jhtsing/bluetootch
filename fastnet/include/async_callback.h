#ifndef __ASYNC_CALLBACK_H__
#define __ASYNC_CALLBACK_H__
#include <string> 
#include <cstdint>
#include <Windows.h>
#include <functional>
#include <memory>
namespace service
{
	struct async_callback_base_t
		: public OVERLAPPED
	{
		async_callback_base_t()
		{
			std::memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));
		}
		virtual ~async_callback_base_t() {}
		virtual void invoke(const std::error_code &error, std::uint32_t size) = 0;
	private:
		async_callback_base_t(const async_callback_base_t &);
		async_callback_base_t &operator=(const async_callback_base_t &);
	};
	typedef std::function<void(const std::error_code&, std::uint32_t)> HandlerType;
	struct  async_callback_t : public async_callback_base_t
	{
		HandlerType handler_;
		async_callback_t(HandlerType handler) :
		handler_(handler)
		{}
		virtual void invoke(const std::error_code &error, std::uint32_t size)
		{
			handler_(error, size);
		}
	};
	typedef std::unique_ptr<async_callback_base_t> async_callback_base_ptr;
	inline async_callback_t* make_callback_ptr(HandlerType handler)
	{
		async_callback_t* ptr = new async_callback_t(handler);
		return ptr;
	} 
}
#endif