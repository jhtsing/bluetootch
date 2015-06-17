#include "pipe_channel.h"

namespace named_pipe
{

	pipe_channel::pipe_channel(service::iocp_impl& _io_service):
	io_service_(_io_service)
	{
	}
	std::error_code pipe_channel::close()
	{

	}
	bool pipe_channel::connect(const std::string& pipe_name)
	{
		handle_ = ::CreateFileA(
			pipe_name.c_str(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
			NULL
			);
		if (handle_ == INVALID_HANDLE_VALUE)
		{
			//return error_code(::GetLastError());
			return false;
		}
		DWORD mode = PIPE_READMODE_BYTE;
		if (!::SetNamedPipeHandleState(handle_, &mode, NULL, NULL))
		{
			int error = ::GetLastError();
			::CloseHandle(pipe_handle);
			return false;
		}
		io_service_.bind(handle_);

		return true; 
	}
	void pipe_channel::async_connect(const std::string& pipeName, connect_handler_type handler, unsigned long timeout_msec)
	{

	}
	void pipe_channel::async_read(read_handler_type msg_handler)
	{
		if (handle_ != INVALID_HANDLE_VALUE)
		{
			msg_handler(pipe_message(), std::error_code());
			return;
		}
		pipe_async_read(handle_, &header_, sizeof(header_), std::bind(pipe_channel::handle_read_header,
			shared_from_this(),
			msg_handler,
			std::placeholders::_1,
			std::placeholders::_2
			));
	}
	void pipe_channel::handle_read_header(
		read_handler_type msg_handler,
		const std::error_code& ec,
		std::uint32_t size)
	{
		if (ec)
		{
			msg_handler(pipe_message(), ec);
			return;
		}
		else
		{
			std::shared_ptr<pipe_message> msg(new pipe_message());
			msg->malloc_when_recvd_header(header_);
			DWORD read_bytes = 0;
			BOOL bret = ReadFile(handle_, &header_, sizeof(header_), &read_bytes, async_result.get()); 

		}
	}
	void pipe_channel::async_write(pipe_message msg, write_handler_type handler)
	{

	}
	bool pipe_channel::is_acceptor_ready(const std::string& pipe) const
	{


	}
	template <typename H> 
	void pipe_async_read(HANDLE pipe_handle, const char * buf, size_t size, H handler)
	{
		service::async_callback_base_ptr async_result(service::make_callback_ptr(handler)));
		DWORD read_bytes = 0;
		BOOL bret = ::ReadFile(pipe_handle, (LPVOID)buf, size, &read_bytes, async_result.get());
		if (!bret&& GetLastError() != ERROR_IO_PENDING)
		{
			async_result->invoke(pipe_message(), std::error_code());
			return;
		} 
		async_result.release();
	}
}