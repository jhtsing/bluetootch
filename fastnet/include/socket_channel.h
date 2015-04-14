#ifndef __SOCKET_CHANNEL_H__
#define __SOCKET_CHANNEL_H__
#include "include\channel.h"
#include "include\iocp_impl.h"
#include "include\socket_handle_t.h"
namespace network
{
class socket_channel :
	public channel,
	public std::enable_shared_from_this<socket_channel>
{
public:
	explicit socket_channel(service::iocp_impl& io_service) :
	io_service_(io_service)
	{
		sock_.reset(new socket_handle_t(io_service_));
	}
	explicit socket_channel(service::iocp_impl& io_service, socket_handle_ptr ptr):
	io_service_(io_service),
	sock_(ptr)
	{
	}
	~socket_channel()
	{
		close();
	}
	virtual bool connect(const end_point& remote_point);

	virtual void async_connect(const end_point& remote_point, connect_handler_type handler) ;
	virtual void async_write(const char* buf, std::uint32_t size, write_handler_type handler);
	virtual void async_read(read_handler_type handler); 
	virtual void close();
private:
	void handle_read_header(read_handler_type handler, message msg, std::error_code ec, std::uint32_t size);
	void handle_read_body(read_handler_type handler, message msg, std::error_code ec, std::uint32_t size);
	socket_handle_ptr sock_;
	service::iocp_impl& io_service_;
}; 
}

#endif 
