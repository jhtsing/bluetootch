#ifndef __SOCKET_HANDLE___
#define __SOCKET_HANDLE___
#include <string>
#include <memory>
#include <functional>
#include "socket_type.h"
#include "ip_address.h"
#include "socket_function.h"
#include "include\iocp_impl.h"
#include "memory_buffer.h"
#pragma comment(lib, "Ws2_32.lib") 
namespace network
{
	class socket_handle_t;
	typedef std::shared_ptr<socket_handle_t> socket_handle_ptr;
	class socket_handle_t :public std::enable_shared_from_this<socket_handle_t>
	{
	public:
		typedef std::function<void (std::error_code,std::uint32_t)> read_handler_type;
		typedef std::function<void (std::error_code,std::uint32_t)> write_handler_type;
		typedef std::function<void (std::error_code)> connect_handler_type;
		typedef std::function<void(std::error_code)> disconnect_handler_type;
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
			return SOCKET_ERROR != ::setsockopt(sock_, option.level(), option.name(), option.data(), option.size());
		}
		template< typename IOCtrlT >
		bool io_control(IOCtrlT &ioCtrl)
		{
			if (!is_open())
			{
				return false;
			}
			DWORD dwRet = 0;
			if (0 != ::WSAIoctl(sock_, ioCtrl.cmd(), ioCtrl.in_buffer(), ioCtrl.in_buffer_size(),
				ioCtrl.out_buffer(), ioCtrl.out_buffer_size(), &dwRet, 0, 0))
			{
				return false;
			}
			return true;
		}

		bool connect(const ip_address& addr, std::uint16_t port);
		void disconnect(int shut, bool bReuseSocket = true);		
		//sync 
		size_t read(mutable_buffer_t &buffer, DWORD flag);
		size_t write(const const_buffer_t &buffer, DWORD flag);
		//async  
		void async_read(mutable_buffer_t &buffer, read_handler_type handler);
		void async_write(const const_buffer_t &buf, write_handler_type handler);
		bool async_disconnect(bool is_use, disconnect_handler_type handler);
		bool async_connect(const end_point& addr, connect_handler_type handler);
	private:
		bool open(int family, int nType, int nProtocol);
		// shutdown
		void shutdown(int shut);
		void handle_connect(connect_handler_type handler,const std::error_code ec, std::uint32_t size);
		void handle_disconnect(connect_handler_type handler, const std::error_code ec, std::uint32_t size);
		SOCKET sock_;
		service::iocp_impl& iocp_service_;
	};
}
#endif 
