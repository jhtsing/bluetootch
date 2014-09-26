#ifndef  __SOCKET_SESSION_H__
#define  __SOCKET_SESSION_H__
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class message;
class socket_channel;
using namespace boost::asio;
class session: public boost::enable_shared_from_this<session>
{
public:
	session(boost::shared_ptr<socket_channel>& c);
	~session(void);
	void async_read();
	void async_write();
	void clear();
private:
	void handle_recv_msg(boost::shared_ptr<message>& msg);
	void handle_read_msg(boost::shared_ptr<message>& msg , boost::system::error_code ec);
	void handle_write_msg(boost::system::error_code ec, size_t len);
	boost::shared_ptr<socket_channel> socket_channel_;
};
#endif 
