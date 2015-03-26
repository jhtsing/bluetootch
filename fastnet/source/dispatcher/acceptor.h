#ifndef __ACCEPT_H__
#define __ACCEPT_H__
#include <functional>
#include "include\iocp_impl.h"
#include "ip_address.h"
#include "socket_handle_t.h"

namespace network
{
	class acceptor
	{
	public:
		acceptor(service::iocp_impl& impl);
		acceptor(service::iocp_impl& impl,const end_point addr);
		~acceptor();
		bool is_open()
		{
			return (sock_ != INVALID_SOCKET);
		}
		template<typename SocketOptionT>
		bool set_option(const SocketOptionT &option)
		{
			if (!is_open())
				return false;
			return SOCKET_ERROR != ::setsockopt(socket_, option.level(), option.name(), option.data(), option.size());
		} 
		bool open(int family, int type  = SOCK_STREAM , int protocol = IPPROTO_TCP);
		void close();
		bool listen(int max);
		bool bind(int family, const ip_address addr, std::uint16_t port);
		void async_accept(std::shared_ptr<socket_handle_t>& sock, )
	private:
		socket_native_type sock_;
		service::iocp_impl& iocp_service_;
	}; 
}


#endif 
