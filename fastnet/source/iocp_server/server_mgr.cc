#include "stdafx.h"
#include "server_mgr.h"
#include <iostream>

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

void server_mgr::async_read(socket_handle_ptr ptr)
{
	if (ptr)
	{
		mutable_buffer_t mutable_buf(&buf_[0], buf_.size());
		ptr->async_read(mutable_buf, std::bind(&server_mgr::handle_read,
			this,
			ptr,
			std::placeholders::_1,
			std::placeholders::_2
			));
	}
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
		std::cout << "recv new accept" << std::endl; 
		async_read(ptr);
	}
}
void server_mgr::handle_read(socket_handle_ptr ptr, std::error_code ec, std::uint32_t size)
{
	if (ec)
	{
		std::cout << "hand read failed" << std::endl;
		return;
	}
	else
	{
		async_read(ptr);
		std::cout << "hand read ok size \t" << size<< std::endl;
		char sz_buf[20] = { 0 };
		memcpy(sz_buf, &buf_[0], size);
		std::cout << "recv content " << sz_buf << std::endl;
	}
}
void server_mgr::stop()
{
	if (acceptor_)
	{
		acceptor_->close();
	}
}