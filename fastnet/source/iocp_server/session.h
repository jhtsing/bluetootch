#ifndef __SESSION_H__
#define __SESSION_H__ 
#include "include\socket_channel.h"
using namespace network;
class session:public std::enable_shared_from_this<session>
{
public:
	session(socket_handle_ptr& ptr);
	~session();
	bool start();
	void stop();
private:
	void async_read();
	void handle_read(std::error_code ec, std::uint32_t size);
	char buf_[100];
	socket_handle_ptr ptr_;
};

#endif 