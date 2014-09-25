#ifndef __ACCEPT_H__
#define __ACCEPT_H__
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class acceptor:public boost::enable_shared_from_this<acceptor>
{
public:
	acceptor(boost::asio::io_service& _io_service);
	~acceptor(void);
	bool start();
	void stop();
private:
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::io_service& io_service_;
};
#endif