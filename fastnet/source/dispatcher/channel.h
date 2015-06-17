#ifndef __CHANNEL_H__
#define __CHANNEL_H__
#include <string>
#include <functional>
#include <Windows.h> 
#include "pipe_message.h"
namespace named_pipe
{
	class channel
	{
	public:
		typedef std::function<void(const pipe_message, std::error_code)> read_handler_type;
		typedef std::function<void(std::error_code, size_t)> write_handler_type;
		typedef std::function<void(std::error_code)> connect_handler_type;
		
		virtual std::error_code close() = 0;
		virtual bool connect(const std::string& pipe_name);
		virtual void async_connect(const std::string& pipeName, connect_handler_type handler, unsigned long timeout_msec = INFINITE) = 0;
		virtual void async_read(read_handler_type msg_handler) = 0;
		virtual void async_write(pipe_message msg, write_handler_type handler) = 0;
		virtual bool is_acceptor_ready(const std::string& pipe)const = 0;
	};
}
#endif 