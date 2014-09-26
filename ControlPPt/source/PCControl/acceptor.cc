#include "acceptor.hpp"
#include <boost/bind.hpp>
#include "socket_accptor.h"
#include "session.h"

acceptor::acceptor(boost::asio::io_service& _io_service):
io_service_(_io_service)
{
}
acceptor::~acceptor(void)
{
}
void acceptor::start()
{
	boost::mutex::scoped_lock lock(mutex_);
	try
	{
		socket_accptor_.reset(new socket_accptor(io_service_));
	}
	catch(std::exception& ec)
	{
		return;
	}
}
void acceptor::stop()
{
	boost::mutex::scoped_lock lock(mutex_);
	if (socket_accptor_)
	{
		socket_accptor_->clear();
		socket_accptor_.reset();
	}
}
void acceptor::async_accept()
{
	try
	{
		if(socket_accptor_)
		{
			socket_accptor_->async_accpet(boost::bind(&acceptor::hande_accept , shared_from_this() , _1, _2)); 
		}
	}
	catch (std::exception& ec)
	{
		return;
	}
}
void acceptor::hande_accept(boost::shared_ptr<socket_channel>& c,boost::system::error_code ec)
{
	if (!ec)
	{
		boost::shared_ptr<session> _session(new session(c));
	}
	else
	{
		c->close();
	}
}