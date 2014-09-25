#ifndef  __SESSION_H__
#define  __SESSION_H__
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

class session : 
	public boost::enable_shared_from_this<session>,
	public boost::noncopyable
{
public:
	session(boost::asio::io_service& in_io_service);
	~session(void);
	bool start();
	bool stop();
	boost::asio::ip::tcp::socket& socket()
	{
		return socket_;
	}
private:
	void OnHandleRead();
	void clear();
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::socket socket_;
};
#endif 
