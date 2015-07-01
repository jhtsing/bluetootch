#include "stdafx.h"
#include "session.h"
#include <iostream>
#include "include\log.h"

session::session(channel_ptr& ptr):
chl_(ptr)
{
}
session::~session()
{
	stop();
}
bool session::start()
{
	async_read();
	return true;
}
void session::stop()
{
	if (chl_)
	{
		chl_->close();
	}
}
void session::async_read()
{
	if (chl_)
	{
		std::cout << "waiting read ......." << std::endl;
		chl_->async_read(std::bind(&session::handle_read,
			shared_from_this(),
			std::placeholders::_1,
			std::placeholders::_2
			)); 
	}
}
void session::async_write(const char* buf, int len)
{
	if (chl_)
	{
		message msg(buf ,len);
		chl_->async_write(msg, std::bind(&session::handle_write,
			shared_from_this(),
			std::placeholders::_1,
			std::placeholders::_2));
	}
}
void session::handle_write(std::error_code ec, std::uint32_t size)
{
	if (!ec)
	{
		base::log_out_a("session handle write %d!\n", size);
	}
}
void session::handle_read(message msg, std::error_code ec)
{
	if (ec)
	{
		printf("handle read failed!\n");
		return;
	}
	else
	{
		std::cout << "hand read ok size \t" << msg.size()<< std::endl;
		char sz_buf[101] = { 0 };
		memcpy(sz_buf, msg.data(), msg.data_size());
		std::cout << "recv content " << sz_buf << std::endl;
		async_read();
		async_write(sz_buf, msg.data_size());
	}
}