#include "include\pipe_acceptor.h"
#include "lowest_sa.h"
#include <algorithm>    // std::for_each 
#include "include\pipe_channel.h"

namespace named_pipe
{

	pipe_acceptor::pipe_acceptor(service::iocp_impl& io_service):
	io_service_(io_service)
	{
		session_ = 0;
	}
	pipe_acceptor::~pipe_acceptor()
	{
		close();
	}
	bool pipe_acceptor::open(const std::string& pipe_name)
	{
		pipe_name_ = pipe_name;
		session_++;
		return true;
	}
	void pipe_acceptor::handle_accept(p_channel::ptr chl,
		p_accept_handler_type handler,
		int session,
		const std::error_code& ec,
		std::uint32_t size)
	{
		{
			std::unique_lock<std::mutex> lock(pendings_mutex_);
			pendings_.insert(chl);
		}
		if (ec)
		{
			handler(p_channel::ptr(), ec);
			return;
		}
		if (session != session_)
		{
			return;
		}
		handler(chl, ec);
	}
	bool pipe_acceptor::close()
	{
		session_++;
		{
			std::unique_lock<std::mutex> lock(pendings_mutex_);
			std::for_each(pendings_.begin(), pendings_.end(), [](p_channel::ptr ptr){
				if(ptr) 
					ptr->close();
			});
			pendings_.clear();
		}
		return true;
	}
	void pipe_acceptor::do_accept(p_accept_handler_type handler)
	{
		std::error_code ec;
		HANDLE pipe = create_pipe(ec);
		if (ec)
		{
			handler(p_channel::ptr(), ec);
			return;
		}
		p_channel::ptr ptr(new pipe_channel(io_service_));
		service::async_callback_base_ptr over_lapped(
			service::make_callback_ptr(
			std::bind(&pipe_acceptor::handle_accept,
			shared_from_this(),
			ptr,
			handler,
			session_,
			std::placeholders::_1,
			std::placeholders::_2))
			);
		BOOL success = ::ConnectNamedPipe(pipe, over_lapped.get());
		DWORD const kLastError = ::GetLastError();
		bool need_callback = false;
		if (!success)
		{
			switch (kLastError)
			{
			case ERROR_PIPE_CONNECTED:
				//ERROR_PIPE_CONNECTED状态时，stream不会回掉。（这是asio的bug??）
				//所以这里主动回掉。
				need_callback = true;
				break;
			case ERROR_IO_PENDING:
				break;
			default://出错了
				std::error_code error(kLastError, std::generic_category());
				if (INVALID_HANDLE_VALUE != pipe)
				{
					::DisconnectNamedPipe(pipe);
					::CloseHandle(pipe);
				}
				over_lapped->invoke(error, 0);
				return;
			}
		}
		if (need_callback)
		{
			handler(ptr, std::error_code());
			return;
		}
		else
		{
			std::unique_lock<std::mutex> lock(pendings_mutex_);
			pendings_.insert(ptr); 
		}
		ptr->be_server(pipe);
		over_lapped.release(); 
	}
	HANDLE pipe_acceptor::create_pipe(std::error_code& ec)
	{
		CLowSA pipeSA;
		void* h = ::CreateNamedPipeA(pipe_name_.c_str()
			, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED
			, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE
			, PIPE_UNLIMITED_INSTANCES
			, (DWORD)buffer_size_
			, (DWORD)buffer_size_
			, NMPWAIT_USE_DEFAULT_WAIT
			, pipeSA.GetSA()
			);
		if (h == INVALID_HANDLE_VALUE)
		{
			std::error_code ec_(GetLastError(), std::generic_category()); 
			ec = ec_;
		}
		return h; 
	}
	void pipe_acceptor::async_accept(p_accept_handler_type handler)
	{
		io_service_.post(
			std::bind(&pipe_acceptor::do_accept,
			shared_from_this(),
			handler));
	}
}