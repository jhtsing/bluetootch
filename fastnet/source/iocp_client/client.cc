#include "stdafx.h"
#include "client.h"
#include <iostream>
clientmgr::clientmgr():
io_service_(1)
{
}
clientmgr::~clientmgr()
{
}
bool clientmgr::init()
{
	chl_ = create_channel_ptr(io_service_);
	if (!chl_)
		return false;
	else
		return true;
}
void clientmgr::uninit()
{
	if (chl_)
	{
		chl_->close();
	}
}
bool clientmgr::connect()
{
	bool bret = false;
	if (chl_)
	{
		ip_address ip_addr = parse("127.0.0.1");
		end_point remote_point(AF_INET, ip_addr, 8001);
		bret = chl_->connect(remote_point);
	}
	return bret;
}
bool clientmgr::send(const char* buf, int len)
{
	if (chl_)
	{
		chl_->async_write(buf, len, 
			std::bind(&clientmgr::handle_write,
			this,
			std::placeholders::_1,
			std::placeholders::_2
			));
		return true;
	}
	else
	{
		return false;
	}
}
void clientmgr::handle_write(std::error_code ec, std::uint32_t size)
{
	if (ec)
	{
		std::cout << "handle wirte failed" << std::endl;
	}
	else
	{
		std::cout << "handle wirte ok size:\t" << size <<std::endl; 
	}
}
void clientmgr::close()
{
	if (chl_)
		chl_->close();
}