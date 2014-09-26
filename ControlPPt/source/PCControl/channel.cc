#include "channel.hpp"
#include <Windows.h>
#include <boost/bind.hpp>


socket_channel::socket_channel(boost::asio::io_service& _io_service):
io_service_(_io_service),
socket_(_io_service)
{
}
socket_channel::~socket_channel()
{
	close();
}
boost::system::error_code socket_channel::close()
{
	boost::system::error_code ec;
	if (socket_.is_open())
	{
		socket_.shutdown(boost::asio::socket_base::shutdown_both);
		socket_.close(ec);
	}
	return ec;
}
void socket_channel::async_read(read_handler_type msg_handler)
{
	try
	{
		//不考虑一个正在等待，另外一个过来读的情况
		boost::shared_ptr<message> msg(new message());
		boost::asio::async_read(socket_ ,
			boost::asio::buffer(msg->buffer() , msg->header_len()),
			boost::asio::transfer_exactly(msg->header_len()),
			boost::bind(&socket_channel::handle_read_header , shared_from_this() , msg_handler , msg ,_1 , _2)
			);
	}
	catch (std::exception& ec)
	{
		return;
	}
}
void socket_channel::async_write(message msg, write_handler_type handler)
{
	try
	{
		boost::asio::async_write(socket_ ,
			boost::asio::buffer(msg.buffer() , msg.size()),
			 boost::asio::transfer_exactly(msg.size()),
			boost::bind(&socket_channel::handle_wirte ,shared_from_this() , handler , _1, _2));
	}
	catch(std::exception& ec)
	{
		close();
	}
}
void socket_channel::handle_read_header(read_handler_type handler , boost::shared_ptr<message> msg , boost::system::error_code ec,size_t size)
{
	if (ec)
	{
		handler(msg, ec);
		close(); 
	}
	else
	{
		BOOST_ASSERT(size = msg->header_len());
		int len = msg->data_len();
		if (len>0)
		{
			msg->alloc(len);
			boost::asio::async_read(socket_ ,
				boost::asio::buffer(msg->data() , msg->data_len()),
				boost::asio::transfer_exactly(msg->header_len()),
				boost::bind(&socket_channel::handle_read_body , shared_from_this() , handler ,msg , _1 , _2)
				);
		}
		else
		{
			handler(msg ,ec);
		}
	}
}
void socket_channel::handle_read_body(read_handler_type handler ,boost::shared_ptr<message>& msg , boost::system::error_code ec, size_t size)
{
	if(ec)
	{
		handler(msg ,ec);
		close();
	}
	else
	{
		BOOST_ASSERT(size == msg->data_len());
		handler(msg ,ec);
	}
}
void socket_channel::handle_wirte(write_handler_type handler , boost::system::error_code ec , size_t len)
{
	handler(ec , len);
}