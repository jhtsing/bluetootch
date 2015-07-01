#include "stdafx.h"
#include <iostream>
#include "pipe_server.h"
#include "pipe_session.h"
#include "include\pipe_acceptor.h"

const std::string k_pipe_name = "\\\\.\\pipe\\my_namedpipe";  
pipe_mgr::pipe_mgr():
io_service_(1)
{

}
pipe_mgr::~pipe_mgr()
{
	uninit();
}
bool pipe_mgr::init()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (!acceptor_)
	{
		acceptor_.reset(new pipe_acceptor(io_service_));
		return false;
	}
	if (!acceptor_)
	{
		return false;
	}
	acceptor_->open(k_pipe_name);
	return true;
}
void pipe_mgr::uninit()
{
	stop();
}
bool pipe_mgr::start()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (acceptor_)
	{
		std::cout << "waiting new accept ......." << std::endl;
		acceptor_->async_accept(
			std::bind(&pipe_mgr::handle_accept, 
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
void pipe_mgr::handle_accept(p_channel::ptr chl, std::error_code ec)
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
		std::shared_ptr<pipe_session> s_(new pipe_session(chl));
		s_->start();
	}
}
void pipe_mgr::stop()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (acceptor_)
	{
		acceptor_->close();
	}
} 
