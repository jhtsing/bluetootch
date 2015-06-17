#ifndef __PIPE_CHANNEL_H__ 
#define __PIPE_CHANNEL_H__
#include <memory>
#include <mutex>
#include "p_channel.h"
#include "include\iocp_impl.h"
namespace named_pipe
{
class pipe_channel :
	public p_channel,
	public std::enable_shared_from_this<pipe_channel>
{
public:
	pipe_channel(service::iocp_impl& _io_service);
	~pipe_channel();
	virtual std::error_code close() ;
	virtual bool connect(const std::string& pipe_name);
	virtual void async_connect(const std::string& pipeName, connect_handler_type handler, unsigned long timeout_msec = INFINITE);
	virtual void async_read(read_handler_type msg_handler);
	virtual void be_server(HANDLE handle);
	virtual void async_write(pipe_message msg, write_handler_type handler) ;
	virtual bool is_acceptor_ready(const std::string& pipe) const; 
private:
	HANDLE& native_handle()
	{
		return handle_;
	}
	const HANDLE& native_handle() const
	{
		return handle_;
	}
	void handle_write(write_handler_type handler,
		const std::error_code& ec,
		std::uint32_t size);
	void handle_read_header(read_handler_type msg_handler, 
		const std::error_code& ec,
		std::uint32_t size);
	void handle_read_body(
		std::shared_ptr<pipe_message> msg,
		read_handler_type msg_handler,
		const std::error_code& ec,
		std::uint32_t size );
	HANDLE handle_;
	bool is_server_;
	pipe_message::Header header_;
	service::iocp_impl& io_service_;
	std::mutex handle_mutex_; 
};
template <typename H>
void pipe_async_read(HANDLE pipe_handle, const char * buf, size_t size, H Handler);

template <typename H>
void pipe_async_write(HANDLE pipe_handle, const char *buf, size_t size, H Handler);

}



#endif 