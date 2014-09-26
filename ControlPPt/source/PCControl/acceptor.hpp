#ifndef __ACCPTOR_H__
#define __ACCPTOR_H__
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>

class socket_accptor;
class socket_channel;
class acceptor:
	public boost::enable_shared_from_this<acceptor>
{
public:
	acceptor(boost::asio::io_service& _io_service);
	~acceptor(void);
	void start();
	void stop();
	void async_accept();
	void hande_accept(boost::shared_ptr<socket_channel>& c,boost::system::error_code ec);
private:
	boost::mutex mutex_;
	boost::asio::io_service& io_service_;
	boost::shared_ptr<socket_accptor> socket_accptor_;
};
#endif 