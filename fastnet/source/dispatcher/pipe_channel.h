#ifndef __PIPE_CHANNEL_H__ 
#define __PIPE_CHANNEL_H__
#include <memory>
#include <mutex>
#include "channel.h"
#include "include\iocp_impl.h"
namespace named_pipe
{
class pipe_channel :
	public channel,
	public std::enable_shared_from_this<pipe_channel>
{
public:
	pipe_channel(service::iocp_impl& _io_service);
	~pipe_channel();
	virtual std::error_code close() ;
	virtual bool connect(const std::string& pipe_name);
	virtual void async_connect(const std::string& pipeName, connect_handler_type handler, unsigned long timeout_msec = INFINITE);
	virtual void async_read(read_handler_type msg_handler);
	virtual void async_write(pipe_message msg, write_handler_type handler) ;
	virtual bool is_acceptor_ready(const std::string& pipe) const; 
private:
	void handle_read_header(read_handler_type msg_handler, const std::error_code& ec, std::uint32_t size);
	std::mutex handle_mutex_;
	HANDLE handle_;
	pipe_message::Header header_;
	service::iocp_impl& io_service_;
};
template <typename H>
void pipe_async_read(HANDLE pipe_handle, const char * buf, size_t size, H Handler);

}



#endif 