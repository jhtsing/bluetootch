#ifndef __ACCEPT_H__
#define __ACCEPT_H__
#include "ip_address.h"
#include "include\iocp_impl.h"
#include "socket_type.h"
#include "socket_handle_t.h"
#include "include\async_callback.h"
namespace network
{
	class socket_acceptor : public std::enable_shared_from_this<socket_acceptor>
{
public:
	typedef std::function<void(socket_handle_ptr ,std::error_code)> accept_handler_type;
	socket_acceptor(service::iocp_impl& io_service, end_point addr);
	~socket_acceptor();
	void close();
	void async_acceptor(socket_handle_ptr& accept_sock_ptr, accept_handler_type handler);
private:
	service::iocp_impl& get_service()
	{
		return io_service_;
	}
	bool is_open()
	{
		return (INVALID_SOCKET != sock_);
	}
	template<typename SocketOptionT>
	bool set_option(const SocketOptionT &option)
	{
		if (!is_open())
			return false;
		return SOCKET_ERROR != ::setsockopt(sock_, option.level(), option.name(), option.data(), option.size());
	}
	void shutdown(int shutdown_type)
	{
		if (!is_open())
		{
			return;
		}
		int bret = ::shutdown(sock_, shutdown_type);
		assert(bret == SOCKET_ERROR);
	}
	bool open(int family, int type, int protocol);
	bool bind(int family, const ip_address &addr, std::uint16_t port);
	bool listen(int max);
	bool start_accept();
	void handle_accept(
		std::vector<char> buffer,
		socket_handle_ptr remote_sock,
		accept_handler_type handler,
		std::error_code ec,
		std::uint32_t size
		);
	end_point addr_;
	socket_native_type sock_;
	service::iocp_impl& io_service_;
}; 

class accept_handler
{
public:
	accept_handler(
		socket_native_type& accept_sock,
		socket_handle_ptr remote_socket,
		socket_acceptor::accept_handler_type handler) :
		accept_socket_(accept_sock),
		remote_sock_(remote_socket),
		handler_(handler)
	{
	}
	~accept_handler();
public:
	void operator()(const std::error_code &error, std::uint32_t size)
	{
		// ¸´ÖÆListen socketÊôÐÔ
		update_accept_context context(accept_socket_);
		remote_sock_->set_option(context);

		handler_(remote_sock_, error);
	} 
	
	socket_handle_ptr remote_sock_;
	socket_native_type accept_socket_;
	char buffer_[MAX_SCOKADDR_BUFFER * 2];
	socket_acceptor::accept_handler_type handler_;
}; 

}
#endif 