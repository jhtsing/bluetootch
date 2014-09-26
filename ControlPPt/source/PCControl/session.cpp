#include "session.h"
#include <boost/bind.hpp>
#include "channel.hpp"
#include "message.hpp"
session::session(boost::shared_ptr<socket_channel>& c):
socket_channel_(c)
{
}
session::~session(void)
{
	clear();
}
void session::clear()
{
	if (socket_channel_)
	{
		//close and destory
		socket_channel_->close();
		socket_channel_.reset();
	}
}
void session::async_read()
{
	if (socket_channel_)
	{
		socket_channel_->async_read(boost::bind(&session::handle_read_msg , shared_from_this() , _1 , _2));
	}
	else
	{
		clear();
	}
}
void session::handle_read_msg(boost::shared_ptr<message>& msg , boost::system::error_code ec)
{
	
}
void session::handle_recv_msg(boost::shared_ptr<message>& msg)
{
	
}
void session::handle_write_msg(boost::system::error_code ec, size_t len)
{
	//do nothing

}