#ifndef __CHANNEL_H__
#define __CHANNEL_H__ 
#include <functional>
#include <memory>
#include "include\ip_address.h"
#include "include\iocp_impl.h"
#include "include\socket_handle_t.h"
#include "message.h"
namespace network
{
class channel
{
public:
	typedef std::function<void(std::error_code)> connect_handler_type;
	typedef std::function<void(message, std::error_code)> read_handler_type;
	typedef std::function<void(std::error_code, std::uint32_t)> write_handler_type;
	virtual ~channel(){}
	virtual bool connect(const end_point& remote_point)=0;
	virtual void async_connect(const end_point& remote_point, connect_handler_type handler)=0;
	virtual void async_write(const message msg, write_handler_type handler) = 0;
	virtual void async_read(read_handler_type handler)=0;
	virtual void close()=0;
}; 
typedef std::shared_ptr<channel> channel_ptr;
channel_ptr create_channel_ptr(service::iocp_impl & io_service);
channel_ptr create_channel_ptr(service::iocp_impl & io_service,socket_handle_ptr ptr);
}
#endif 
