#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "message.hpp"

class socket_channel:public boost::enable_shared_from_this<socket_channel>
{
	typedef boost::function<void(boost::shared_ptr<message>&,boost::system::error_code)> read_handler_type;
	typedef boost::function<void(boost::system::error_code,size_t)> write_handler_type;
	typedef boost::function<void(boost::system::error_code)> connect_handler_type;
public:
	socket_channel(boost::asio::io_service& in_io_service);
	~socket_channel(void);
	virtual boost::system::error_code close();
	virtual void async_read(read_handler_type msg_handler);
	virtual void async_write(message msg, write_handler_type handler);
	virtual  boost::asio::ip::tcp::socket& socket()
	{
		return socket_;
	}
private:
	void handle_read_header(read_handler_type handler , boost::shared_ptr<message> msg , boost::system::error_code ec,size_t len);
	void handle_read_body(read_handler_type handler ,boost::shared_ptr<message>& msg , boost::system::error_code, size_t len);
	void handle_wirte(write_handler_type handler ,boost::system::error_code ec , size_t len);
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::socket socket_;
	
}; 

#endif 