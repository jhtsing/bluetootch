#include "acceptor.h"

namespace network
{

	acceptor::acceptor(service::iocp_impl& impl):
	iocp_service_(impl),
	sock_(INVALID_SOCKET)
	{}
	acceptor::acceptor(service::iocp_impl& impl, const end_point addr):
	iocp_service_(impl),
	sock_(INVALID_SOCKET)
	{
	}
	acceptor::~acceptor()
	{
	}
	bool acceptor::open(int family, int type , int protocol )
	{
		sock_ = ::socket(family, type, protocol);
		return (sock_ != INVALID_SOCKET);
	}
	void acceptor::close()
	{

	}
	bool acceptor::listen(int max)
	{
		if (!is_open())
		{
			return false;
		}
		bool ret = ::listen(sock_, max);
		return ret;
	}
	bool acceptor::bind(int family, const ip_address addr, std::uint16_t port)
	{
		if (!is_open())
		{
			return false;
		}
		SOCKADDR_IN sock_addr = { 0 };
		sock_addr.sin_family = family;
		sock_addr.sin_addr.s_addr = ::htonl(addr.address());
		sock_addr.sin_port = ::htons(port);
		int ret = ::bind(sock_ , reinterpret_cast<SOCKADDR*>(&sock_addr), sizeof(SOCKADDR));
		return (ret!=INVALID_SOCKET); 
	}

}

