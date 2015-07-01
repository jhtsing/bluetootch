#include "stdafx.h"
#include "server_mgr.h"
#include <iostream>
#include "session.h"
using namespace network;
server_mgr::server_mgr():
io_service_(1),
buf_(100)
{
}
server_mgr::~server_mgr()
{
}
bool server_mgr::init()
{
	ip_address ip_addr = parse("127.0.0.1");
	end_point bind_point(AF_INET, ip_addr, 8001); 
	acceptor_.reset(new network::socket_acceptor(io_service_, bind_point));
	if (!acceptor_)
	{
		return false;
	}
	return true;
}
void server_mgr::uninit()
{
	stop();
}
bool server_mgr::start()
{
	if (acceptor_)
	{
		std::cout << "waiting new accept ......." << std::endl;
		socket_handle_ptr remote_socket(new socket_handle_t(io_service_));
		acceptor_->async_acceptor(remote_socket, std::bind(&server_mgr::handle_accept,
			this,
			std::placeholders::_1,
			std::placeholders::_2
			));
	}
	else
	{
		return false;
	}
	return true;
}

void server_mgr::handle_accept(socket_handle_ptr ptr, std::error_code ec)
{
	if (ec)
	{
		std::cout << "handle accept failed" << std::endl;
		stop();
		return;
	}
	else
	{
		start();
		std::cout << "recv new accept" << std::endl;
		channel_ptr chl(new socket_channel(io_service_, ptr));
		std::shared_ptr<session> s_(new session(chl));
		s_->start();
	}
}
void server_mgr::stop()
{
	if (acceptor_)
	{
		acceptor_->close();
	}
}