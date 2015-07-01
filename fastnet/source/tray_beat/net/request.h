#ifndef __NET_REQUEST_H__
#define __NET_REQUEST_H__
#include "include\channel.h"
#include "include\socket_init.h"
#include <memory>
using namespace network;
 
class net_request;
typedef std::shared_ptr<net_request> request_ptr;
class net_request
{
public:
	net_request(
		service::iocp_impl& io_service, 
		const std::string& ip_addr, 
		int port);
	~net_request();
	bool init();
	void uninit();
	bool connect();
	template <typename WriteHandler >
	void async_write(const char* buf, int len, WriteHandler handler)
	{
		if (chl_)
		{
			message msg(buf, len);
			chl_->async_write(msg, handler);
		}
		else
		{
			//handler();
		}
	}
	template <typename ReadHandler >
	void async_read(ReadHandler handler)
	{
		if (chl_)
		{
			chl_->async_read(handler);
		}
	}
	void close(); 
private:
	std::string ip_addr_;
	int port_;
	channel_ptr chl_;
	service::iocp_impl& io_service_;
};
#endif 