#include "stdafx.h"
#include "pipe_session.h"
#include <iostream>

pipe_session::pipe_session(p_channel::ptr& ptr) :
ptr_(ptr)
{
}
pipe_session::~pipe_session()
{
	stop();
}
bool pipe_session::start()
{
	async_read();
	return true;
}
void pipe_session::stop()
{
	if (ptr_)
	{
		ptr_->close();
	}
}
void pipe_session::async_read()
{
	if (ptr_)
	{
		std::cout << "waiting read ......." << std::endl;
		ptr_->async_read(
			std::bind(&pipe_session::handle_read,
			shared_from_this(),
			std::placeholders::_1,
			std::placeholders::_2
			));
	}
}
void pipe_session::handle_read(const std::shared_ptr<pipe_message> msg, std::error_code ec)
{
	if (ec||!msg)
	{
		printf("handle read failed!\n");
		return;
	}
	else
	{
		std::cout << "hand read ok size \t"<< std::endl;
		char sz_buf[101] = { 0 };
		memcpy(sz_buf, msg->data(), msg->size());
		std::cout << "recv content " << sz_buf << std::endl;
		async_read();
	}
}