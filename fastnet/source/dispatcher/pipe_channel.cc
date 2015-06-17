#include "include\pipe_channel.h"
#include "include\async_callback.h"
namespace named_pipe
{
	pipe_channel::pipe_channel(service::iocp_impl& _io_service):
	io_service_(_io_service),
	is_server_(false)
	{
	}
	pipe_channel::~pipe_channel()
	{
		close();
	}
	std::error_code pipe_channel::close()
	{
		if (native_handle() != INVALID_HANDLE_VALUE)
		{
			::CancelIo(native_handle());
			if (is_server_)
				::DisconnectNamedPipe(native_handle());
		} 
		return std::error_code();
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
			::CloseHandle(handle_);
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
		if (handle_ == INVALID_HANDLE_VALUE)
		{
			msg_handler(std::shared_ptr<pipe_message>(), std::error_code());
			return;
		}
		pipe_async_read(handle_, (const char *)&header_, sizeof(header_), 
			std::bind(&pipe_channel::handle_read_header,
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
			msg_handler(std::shared_ptr<pipe_message>(), ec);
			return;
		}
		else
		{
			assert(size == sizeof(header_));
			std::shared_ptr<pipe_message> msg(new pipe_message());
			msg->malloc_when_recvd_header(header_);
			pipe_async_read(handle_,
				msg->data(), 
				msg->size(),
				std::bind(&pipe_channel::handle_read_body,
				shared_from_this(),
				msg,
				msg_handler,
				std::placeholders::_1,
				std::placeholders::_2
				));
		}
	}
	void pipe_channel::handle_read_body(
		std::shared_ptr<pipe_message> msg,
		read_handler_type msg_handler,
		const std::error_code& ec,
		std::uint32_t size)
	{
		if (ec||size==0)
		{
			msg_handler(msg, ec);
			return;
		}
		else
		{
			assert(size == msg->size());
			msg_handler(msg, ec);
		}
	}
	void pipe_channel::async_write(pipe_message msg, write_handler_type handler)
	{
		if (handle_ == INVALID_HANDLE_VALUE)
		{
			handler(std::error_code(), 0);
			return;
		}
		pipe_async_write(handle_, (const char *)msg.buffer(), msg.buffer_size(),
			std::bind(&pipe_channel::handle_write,
			shared_from_this(),
			handler,
			std::placeholders::_1,
			std::placeholders::_2)
			);
	}
	void pipe_channel::handle_write(
		write_handler_type handler,
		const std::error_code& ec,
		std::uint32_t size)
	{
		if (ec)
		{
			handler(ec, 0);
			return;
		}
		handler(ec, size);
	}
	void pipe_channel::be_server(HANDLE handle)
	{
		handle_ = handle;
		is_server_ = true;
		io_service_.bind(handle);
	}
	bool pipe_channel::is_acceptor_ready(const std::string& pipe_name) const
	{
		if (WaitNamedPipeA(pipe_name.c_str(), 1))
			return true;
		// If ERROR_SEM_TIMEOUT occurred, the pipe exists but is handling another connection.
		return GetLastError() == ERROR_SEM_TIMEOUT; 
	}
	template <typename H> 
	void pipe_async_read(HANDLE pipe_handle, const char * buf, size_t size, H handler)
	{
		service::async_callback_base_ptr async_result(service::make_callback_ptr(handler));
		DWORD read_bytes = 0;
		BOOL bret = ::ReadFile(pipe_handle, (LPVOID)buf, size, &read_bytes, async_result.get());
		if (!bret&& GetLastError() != ERROR_IO_PENDING)
		{
			async_result->invoke(std::error_code(),0);
			return;
		} 
		async_result.release();
	}
	template <typename H>
	void pipe_async_write(HANDLE pipe_handle, const char *buf, size_t size, H handler)
	{
		service::async_callback_base_ptr async_result(service::make_callback_ptr(handler));
		DWORD write_bytes = 0;
		BOOL bret = ::WriteFile(pipe_handle, buf, size, &write_bytes, async_result.get());
		if (!bret&&::GetLastError() !=ERROR_IO_PENDING)
		{
			async_result->invoke(std::error_code(), 0);
			return;
		}
		async_result.release();
	}
}