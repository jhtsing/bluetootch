#include "socket.hpp"

namespace bluetooch
{

	  
	socket_handle::socket_handle():
	socket_(INVALID_SOCKET)
	{
	}
	socket_handle::~socket_handle()
	{
	}
	bool socket_handle::open(int family ,int nType,  int nProtocol)
	{
		if (is_open())
		{
			return true;
		}
		socket_ = ::WSASocket(family , nType , nProtocol , 0 , 0 , WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == socket_)
		{
			return false;
		}
		return true;
	}
	bool socket_handle::bind(int family ,const ip_address& addr , size_t port)
	{
		if (!is_open())
		{
			return false;
		}
		sockaddr_in socket_addr;
		socket_addr.sin_family = family;
		socket_addr.sin_port = ::htons(port);
		socket_addr.sin_addr = ::htonl(addr.address());
		if (SOCKET_ERROR == ::bind(socket_ , (const SOCKADDR *)&socket_addr , sizeof(SOCKADDR)))
		{
			return false;
		}
		return true;
	}
	bool socket_handle::is_open()
	{
		return (socket_!=INVALID_SOCKET);
	}
	bool socket_handle::listen(int max)
	{
		if (!is_open())
		{
			return false;
		}
		return (SOCKET_ERROR!=::listen(socket_ , max));
	}
	void socket_handle::connect(int family, const ip_address& addr , size_t port)
	{
		sockaddr_in connect_addr;
		connect_addr.sin_family = family;
		connect_addr.sin_addr = ::htonl(addr.address());
		connect_addr.sin_port = ::htons(port);
		return (SOCKET_ERROR!=::connect(socket_ , (const sockaddr*)&connect_addr , sizeof(connect_addr)));
	}
	void socket_handle::disconnect(int shut_type, bool is_reuse)
	{
		if (!is_open())
		{
			return;
		}
		int ret = ::shutdown(socket_, shut_type);
		if (is_reuse)
		{
			//socket ∏¥”√ 
		}
	}
	void socket_handle::close()
	{
		if (!is_open())
		{
			return;
		}
		int ret = ::closesocket(socket_);
		socket_ = INVALID_SOCKET;
	}

}
#endif