#ifndef __SOCKET_HANDLE___
#define __SOCKET_HANDLE___
#include <string>
#include "socket_type.h"
#include "ip_address.h"
#include "socket_function.h"
#include "include\iocp_impl.h"

namespace network
{
	typedef std::unique_ptr<socket_handle_t> socket_handle_ptr;
	class socket_handle_t
	{
	public:
		typedef SOCKET socket_native_type;
		socket_handle_t(service::iocp_impl& iocp_service);
		socket_handle_t(service::iocp_impl& iocp_service,socket_native_type sock);
		socket_handle_t(const socket_handle_t &rhs); 
		socket_handle_t &operator=(const socket_handle_t &rhs);

		~socket_handle_t();
		// WSASocket
		operator socket_native_type&() { return sock_;}
		operator const socket_native_type&() const { return sock_;}
		socket_native_type& native_handle(){
			return sock_;
		}
		const socket_native_type& native_handle() const{
			return sock_;
		}
		bool is_open() const
		{
			return sock_ != INVALID_SOCKET;
		} 
		template<typename SocketOptionT>
		bool set_option(const SocketOptionT &option)
		{
			if (!is_open())
				return false;
			return SOCKET_ERROR != ::setsockopt(socket_, option.level(), option.name(), option.data(), option.size());
		} 
		void set_socket();
		bool open(int family, int nType, int nProtocol);
		// shutdown
		void shutdown(int shut); 
		bool connect(int family , const ip_address& addr, std::uint16_t port);
		void disconnect(int shut, bool bReuseSocket = true);
		bool listen(int max = SOMAXCONN);
		// bind
		void bind(int family, const ip_address &addr, std::uint16_t port);
		// listen
		socket_handle_ptr accept()
		{
			return nullptr;
		}
		

	public:
		SOCKET sock_;
		service::iocp_impl& iocp_service_;
	};

}

#endif 
