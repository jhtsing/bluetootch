#ifndef __CHANNEL_H__
#define __CHANNEL_H__
#include <string>
#include <functional>
#include <Windows.h> 
#include <memory>
#include "pipe_message.h"
namespace named_pipe
{
	class p_channel
	{
	public:
		typedef std::shared_ptr<p_channel> ptr;
		typedef std::function<void(const std::shared_ptr<pipe_message>, std::error_code)> read_handler_type;
		typedef std::function<void(std::error_code, size_t)> write_handler_type;
		typedef std::function<void(std::error_code)> connect_handler_type;
		
		virtual std::error_code close() = 0;
		virtual void be_server(HANDLE handle) = 0;
		virtual bool connect(const std::string& pipe_name)=0;
		virtual void async_connect(const std::string& pipeName, connect_handler_type handler, unsigned long timeout_msec = INFINITE) = 0;
		virtual void async_read(read_handler_type msg_handler) = 0;
		virtual void async_write(pipe_message msg, write_handler_type handler) = 0;
		virtual bool is_acceptor_ready(const std::string& pipe)const = 0;
	};

	class p_acceptor
	{
	public:
		typedef std::function<void(p_channel::ptr, std::error_code)> p_accept_handler_type;
		virtual bool open(const std::string& pipe_name)=0;
		virtual bool close() = 0;
		virtual void async_accept(p_accept_handler_type handler) = 0;
	};
}
#endif 