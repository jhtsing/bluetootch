#include "socket_handle_t.h"

namespace network
{
	socket_handle_t::socket_handle_t(service::iocp_impl& iocp_service):
	iocp_service_(iocp_service),
	sock_(INVALID_SOCKET)
	{
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
		assert(bret == SOCKET_ERROR);
	}
	bool socket_handle_t::connect(int family, const ip_address& addr, std::uint16_t port)
	{
		if (!is_open())
		{
			return false;
		}
		SOCKADDR_IN server_addr = { 0 };
		server_addr.sin_family = family;
		server_addr.sin_addr.s_addr = ::htonl(addr.address());
		server_addr.sin_port = ::htons(port);
		if (SOCKET_ERROR == ::connect(sock_, reinterpret_cast<SOCKADDR *>(&server_addr), sizeof(SOCKADDR_IN)))
		{
			return false;
		}
		return true;
	}
	void socket_handle_t::disconnect(int shut, bool reuse_socket = true)
	{
		if (!is_open)
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
	bool socket_handle_t::listen(int max)
	{
		if (!is_open())
		{
			return false;
		}
		int ret = ::listen(sock_, max);
		if (ret == SOCKET_ERROR)
		{
			return false;
		}
		return true;
	}
	// bind
	void socket_handle_t::bind(int family, const ip_address &addr, std::uint16_t port)
	{
		if (!is_open())
		{
			return;
		}
		SOCKADDR_IN local_addr = { 0 };
		local_addr.sin_family = family;
		local_addr.sin_addr.s_addr = ::htonl(addr.address());
		local_addr.sin_port = ::htons(port);
		int ret = ::bind(sock_, reinterpret_cast<SOCKADDR *>(&local_addr), sizeof(SOCKADDR_IN));
		assert(ret != SOCKET_ERROR);
	}
	// accept


	bool a




}

