#include "include\acceptor.h"


namespace network
{
	socket_acceptor::socket_acceptor(service::iocp_impl& io_service, end_point addr):
	io_service_(io_service),
	addr_(addr),
	sock_(INVALID_SOCKET)
	{
		if (!start_accept())
		{
			assert(false);
		}
	}
	bool socket_acceptor::start_accept()
	{
		if (!open(AF_INET, SOCK_STREAM, IPPROTO_TCP))
		{
			return false;
		}
		reuse_addr reuse(true);
		set_option(reuse);
		if (!bind(addr_.family_, addr_.addr_, addr_.port_))
		{	
			return false;
		}
		if (!listen(SOMAXCONN))
		{
			return false;
		}
		return true;
	}
	socket_acceptor::~socket_acceptor()
	{
	}
	bool socket_acceptor::open(int family, int type, int protocol)
	{
		if (is_open())
		{
			return false;
		}
		sock_ = ::WSASocket(family, type, protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (sock_ == INVALID_SOCKET){
			return false;
		}
		io_service_.bind(reinterpret_cast<HANDLE>(sock_));
		//vista succussful
		if (!::SetFileCompletionNotificationModes(reinterpret_cast<HANDLE>(sock_), FILE_SKIP_COMPLETION_PORT_ON_SUCCESS)){
			return false;
		}
		return true; 
	}
	void socket_acceptor::close()
	{
		shutdown(SD_BOTH);
		reuse_addr reuse(true);
		set_option(reuse);
	}
	bool socket_acceptor::bind(int family, const ip_address &addr, std::uint16_t port)
	{
		if (!is_open())
		{
			return false;
		} 
		SOCKADDR_IN sock_addr = { 0 };
		sock_addr.sin_family = family;
		sock_addr.sin_addr.s_addr = ::htonl(addr.address());
		sock_addr.sin_port = ::htons(port);
		int bret = ::bind(sock_, reinterpret_cast<SOCKADDR *>(&sock_addr), sizeof(SOCKADDR));
		assert(bret != INVALID_SOCKET);
		return (bret != INVALID_SOCKET);
	}
	bool socket_acceptor::listen(int max)
	{
		if (!is_open())
		{
			return false;
		}
		int ret = ::listen(sock_, max);
		assert(ret != SOCKET_ERROR);
		return (ret != SOCKET_ERROR);
	}
	void socket_acceptor::async_acceptor(socket_handle_ptr& remote_sock_ptr, 
		accept_handler_type handler)
	{
		if (!is_open())
		{
			return;
		}
		memset(&addr_buffer_[0], 0, sizeof(addr_buffer_));
		service::async_callback_base_ptr async_result_ptr(service::make_callback_ptr(
			std::bind(&socket_acceptor::handle_accept, 
			shared_from_this(),
			remote_sock_ptr, 
			handler, 
			std::placeholders::_1,
			std::placeholders::_2)
			));
		DWORD dwRecvBytes = 0;
		DWORD error = 0;
		BOOL bret = socket_function::singleton().AcceptEx(
			sock_,
			remote_sock_ptr->native_handle(),
			&addr_buffer_[0],
			0,
			MAX_SCOKADDR_BUFFER,
			MAX_SCOKADDR_BUFFER,
			&dwRecvBytes,
			async_result_ptr.get());
		if (!bret&&((error = WSAGetLastError()) != ERROR_IO_PENDING))
		{
			handler(remote_sock_ptr, std::make_error_code((std::errc)error));
			return;
		}
		async_result_ptr.release();
	}
	void socket_acceptor::handle_accept(
		socket_handle_ptr remote_sock,
		accept_handler_type handler,
		std::error_code ec, 
		std::uint32_t size)
	{
		//new socket accpet 
		update_accept_context context(sock_);
		remote_sock->set_option(context);
		handler(remote_sock, ec);
	}

}