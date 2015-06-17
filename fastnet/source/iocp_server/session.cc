#include "stdafx.h"
#include "session.h"
#include <iostream>

session::session(socket_handle_ptr& ptr):
ptr_(ptr)
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
	if (ptr_)
	{
		ptr_->disconnect(2);
	}
}
void session::async_read()
{
	if (ptr_)
	{
		std::cout << "waiting read ......." << std::endl;
		memset(buf_, 0, sizeof(buf_));
		mutable_buffer_t mutable_buf(buf_, 100);
		ptr_->async_read(mutable_buf, std::bind(&session::handle_read,
			shared_from_this(),
			std::placeholders::_1,
			std::placeholders::_2
			)); 
	}
}
void session::handle_read(std::error_code ec, std::uint32_t size)
{
	if (ec || size == 0)
	{
		printf("handle read failed!\n");
		return;
	}
	else
	{
		std::cout << "hand read ok size \t" << size << std::endl;
		char sz_buf[101] = { 0 };
		memcpy(sz_buf, &buf_[0], size);
		std::cout << "recv content " << sz_buf << std::endl;
		async_read();
	}
}