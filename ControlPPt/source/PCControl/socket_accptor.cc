#include "socket_accptor.h"
#include <boost/bind.hpp>
#include "channel.hpp"

socket_accptor::socket_accptor(boost::asio::io_service& in_io_service):
io_service_(in_io_service),
acceptor_(io_service_,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), g_proxy_server_port))
{
}
socket_accptor::~socket_accptor(void)
{
	clear();
}
bool socket_accptor::async_accpet(accept_handler_type handler)
{
	try
	{
		boost::shared_ptr<socket_channel> _channel(new socket_channel(io_service_));
		acceptor_.async_accept(_channel->socket(), boost::bind(&socket_accptor::handle_accept, shared_from_this(), handler ,_channel ,_1));
	}
	catch (std::exception& e)
	{
		return false;
	}
	return true;
}
void socket_accptor::handle_accept(accept_handler_type handler,
									boost::shared_ptr<socket_channel>& _socket,
									boost::system::error_code ec)
{
	handler(_socket , ec);
}
void socket_accptor::clear()
{
	acceptor_.close();
}