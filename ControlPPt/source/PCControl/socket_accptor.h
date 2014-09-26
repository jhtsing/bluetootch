#ifndef __SOCKT_ACCEPT_H__
#define __SOCKT_ACCEPT_H__
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "channel.hpp"

const std::string g_proxy_server_address = "127.0.0.1";
const short g_proxy_server_port  = 6901;

class socket_accptor:
	public boost::enable_shared_from_this<socket_accptor>
{
	typedef boost::function<void (boost::shared_ptr<socket_channel>&, boost::system::error_code)> accept_handler_type;
public:
	socket_accptor(boost::asio::io_service& _io_service);
	~socket_accptor(void); 
	bool async_accpet(accept_handler_type handler);
	void clear();
private:
	void handle_accept(
		accept_handler_type handler,
		boost::shared_ptr<socket_channel>& _socket,
		boost::system::error_code ec);
	boost::asio::io_service& io_service_; 
	boost::asio::ip::tcp::acceptor acceptor_;  
};
#endif
