#include "include\socket_handle_t.h"

namespace network
{
	socket_handle_t::socket_handle_t(service::iocp_impl& iocp_service):
	iocp_service_(iocp_service),
	sock_(INVALID_SOCKET)
	{
		open(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	socket_handle_t::socket_handle_t(service::iocp_impl& iocp_service, socket_native_type sock) :
	iocp_service_(iocp_service),
	sock_(sock)
	{
		
	}
	socket_handle_t::socket_handle_t(const socket_handle_t &rhs):
	iocp_service_(rhs.iocp_service_),
	sock_(rhs.sock_)
	{
	}
	socket_handle_t &socket_handle_t::operator=(const socket_handle_t &rhs)
	{
		if (&rhs != this)
		{
			sock_ = rhs.sock_;
		}
		return *this;
	} 
	socket_handle_t::~socket_handle_t()
	{
	}
	bool socket_handle_t::open(int family, int type, int protocol)
	{
		if (is_open())
		{
			return false;
		}
		sock_ = ::WSASocket(family, type, protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (sock_==INVALID_SOCKET)
		{
			return false;
		}
		iocp_service_.bind(reinterpret_cast<HANDLE>(sock_));
		//vista succussful
		if (!::SetFileCompletionNotificationModes(reinterpret_cast<HANDLE>(sock_), FILE_SKIP_COMPLETION_PORT_ON_SUCCESS))
		{
			return false;
		}
		return true;
	}
	// shutdown
	void socket_handle_t::shutdown(int shut)
	{
		if (!is_open())
		{
			return;
		}
		int bret = ::shutdown(sock_, shut); 
		DWORD error_code = WSAGetLastError();
		//assert(bret == SOCKET_ERROR);
	}
	bool socket_handle_t::connect(const ip_address& addr, std::uint16_t port)
	{
		if (!is_open())
		{
			return false;
		}
		SOCKADDR_IN server_addr = { 0 };
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = ::htonl(addr.address());
		server_addr.sin_port = ::htons(port);
		if (SOCKET_ERROR == ::connect(sock_, reinterpret_cast<SOCKADDR *>(&server_addr), sizeof(SOCKADDR_IN)))
		{
			DWORD error = GetLastError();
			printf("connect failed error_code %d!\n", error);
			return false;
		}
		return true;
	}
	void socket_handle_t::disconnect(int shut, bool reuse_socket)
	{
		if (!is_open())
		{
			return;
		}
		shutdown(shut);
		if (reuse_socket)
		{
			reuse_addr reuse(true);
			set_option(reuse);
		}
	}
	void socket_handle_t::async_read(mutable_buffer_t &buffer, read_handler_type handler)
	{
		if (!is_open())
		{
			handler(std::error_code(), 0);
			return;
		}
		WSABUF wsabuf = { 0 };
		wsabuf.buf = buffer.data();
		wsabuf.len = buffer.size();
		service::async_callback_base_ptr async_result(service::make_callback_ptr(handler));
		DWORD dwFlag = 0;
		DWORD dwSize = 0; 
		int bret = ::WSARecv(sock_, &wsabuf, 1, &dwSize, &dwFlag, async_result.get(), NULL);
		DWORD code = WSAGetLastError();
		if (bret != 0 && code != WSA_IO_PENDING)
		{
			handler(std::error_code(), 0);
			return ;
		}
		else if (bret ==0)
		{
			async_result->invoke(std::error_code(), dwSize); 
		}
		else
		{
			async_result.release();
		}
	}
	void socket_handle_t::async_write(const const_buffer_t &buf, write_handler_type handler)
	{
		if (!is_open())
		{
			handler(std::error_code(), 0);
			return;
		}
		WSABUF wsabuf = { 0 };
		wsabuf.buf = const_cast<char *>(buf.data());
		wsabuf.len = buf.size();
		DWORD dwflag = 0;
		DWORD dwsize = 0;
		service::async_callback_base_ptr async_result(service::make_callback_ptr(handler));
		int ret = ::WSASend(sock_, &wsabuf, 1, &dwsize, dwflag, async_result.get(), NULL);
		int erroc = WSAGetLastError();
		if ((0 != ret) && erroc != WSA_IO_PENDING)
		{
			handler(std::error_code(erroc , std::generic_category()), 0);
			return;
		}
		else if (0==ret)
		{
			async_result->invoke(std::error_code(), dwsize);
		}
		else
		{
			async_result.release();
		}
	}
	bool socket_handle_t::async_connect(const end_point& addr, connect_handler_type handler)
	{
		if (!is_open())
		{
			return false;
		}
		sockaddr_in local_addr = { 0 };
		local_addr.sin_family = AF_INET;
		// 很变态，需要先bind
		int ret = ::bind(sock_, reinterpret_cast<const sockaddr *>(&local_addr), sizeof(local_addr));
		if (ret != 0)
		{
			printf("bind failed!\n");
			return false;
		}
		sockaddr_in remote_addr = { 0 };
		remote_addr.sin_family = AF_INET;
		remote_addr.sin_port = ::htons(addr.port_);
		remote_addr.sin_addr.s_addr = ::htonl(addr.addr_.address());
		service::async_callback_base_ptr async_result
			(service::make_callback_ptr(std::bind(&socket_handle_t::handle_connect ,
			shared_from_this(),
			handler,
			std::placeholders::_1, 
			std::placeholders::_2)));
		if (!socket_function::singleton().ConnectEx(
			sock_,
			reinterpret_cast<SOCKADDR *>(&remote_addr),
			sizeof(SOCKADDR), 0, 0, 0, async_result.get())
			&& ::WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("connection failed!\n");
			return false;
		}
		async_result.release(); 
		return true;
	}
	void socket_handle_t::handle_connect(connect_handler_type handler,const std::error_code ec, std::uint32_t size)
	{
		handler(ec);
	}
	size_t socket_handle_t::read(mutable_buffer_t &buffer, DWORD flag)
	{
		if (!is_open())
		{
			return 0;
		}
		WSABUF wsabuf = { 0 };
		wsabuf.buf = buffer.data();
		wsabuf.len = buffer.size();
		if (wsabuf.len == 0)
		{
			return 0;
		}
		DWORD dwSize = 0;
		if (0 != ::WSARecv(sock_, &wsabuf, 1, &dwSize, &flag, 0, 0))
		{
			return 0;
		}
		return dwSize; 
	}
	size_t socket_handle_t::write(const const_buffer_t &buffer, DWORD flag)
	{
		if (!is_open())
		{
			return 0;
		}
		WSABUF wsabuf = { 0 };
		wsabuf.buf = const_cast<char *>(buffer.data());
		wsabuf.len = buffer.size();
		if (wsabuf.len == 0)
		{
			return 0;
		}
		DWORD dwSize = 0;
		if (0 != ::WSASend(sock_, &wsabuf, 1, &dwSize, flag, 0, 0))
		{
			return 0;
		}
		return dwSize; 
	}
	bool socket_handle_t::async_disconnect(bool is_reuse, disconnect_handler_type handler)
	{
		if (!is_open())
		{
			return false;
		}
		service::async_callback_base_ptr 
			async_result(service::make_callback_ptr(
			std::bind(&socket_handle_t::handle_disconnect,
			shared_from_this(),
			handler,
			std::placeholders::_1,
			std::placeholders::_2
			)));
		DWORD dwFlags = is_reuse ? TF_REUSE_SOCKET : 0;
		if (!socket_function::singleton().DisconnectEx(sock_, async_result.get(), dwFlags, 0)
			&& ::WSAGetLastError() != WSA_IO_PENDING)
		{
			async_result->invoke(std::make_error_code((std::errc)(::WSAGetLastError())), 0);
			return false;
		}
		async_result.release();
		return true;
	}
	void socket_handle_t::handle_disconnect(connect_handler_type handler, const std::error_code ec, std::uint32_t size)
	{
		handler(ec);
	}
}

