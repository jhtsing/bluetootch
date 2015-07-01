#include "request.h"

network::socket_init<2, 0> sock_init_;
net_request::net_request(
	service::iocp_impl& io_service,
	const std::string& ip_addr,
	int port)
	:io_service_(io_service)
	,ip_addr_(ip_addr)
	,port_(port)
{
}
net_request::~net_request()
{
	uninit();
}
bool net_request::init()
{
	if (!chl_)
	{
		chl_ = create_channel_ptr(io_service_);
	}
	return connect();
}
void net_request::uninit()
{
	close();
}
bool net_request::connect()
{
	bool bret = false;
	if (chl_)
	{
		ip_address ip_addr = parse(ip_addr_);
		end_point remote_point(AF_INET, ip_addr, port_);
		bret = chl_->connect(remote_point);
	}
	return bret; 
}
void net_request::close()
{
	if (chl_)
	{
		chl_->close();
	}
}