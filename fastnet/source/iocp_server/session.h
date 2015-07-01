#ifndef __SESSION_H__
#define __SESSION_H__ 
#include "include\socket_channel.h"
using namespace network;
class session:public std::enable_shared_from_this<session>
{
public:
	session(channel_ptr& _chl);
	~session();
	bool start();
	void stop();
private:
	void async_read();
	void async_write(const char* buf ,int len);
	void handle_write(std::error_code ec, std::uint32_t size);
	void handle_read(message msg, std::error_code ec);
	char buf_[100];
	channel_ptr chl_;
};

#endif 