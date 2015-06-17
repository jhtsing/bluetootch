#include "stdafx.h"
#include "pipe_client.h"
#include "include\pipe_channel.h"
pipe_client::pipe_client():
io_service_(1)
{
}
pipe_client::~pipe_client()
{
	close();
}
void pipe_client::close()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (chl_)
	{
		chl_->close();
	}
}
bool pipe_client::init()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (!chl_)
	{
		chl_.reset(new pipe_channel(io_service_));
	}
	if (!chl_)
	{
		return false;
	}
	return true;
}
bool pipe_client::connect(const std::string& name)
{
	std::unique_lock<std::mutex> lock(mutex_); 
	if (!chl_)
	{
		return false;
	}
	bool bret =  chl_->connect(name);
	if (!bret)
	{
		return false;
	}
	return true;
}
bool pipe_client::send(const char* buf, int size)
{
	std::unique_lock<std::mutex> lock(mutex_); 
	if (chl_)
	{
		pipe_message msg(1, buf, size);
		chl_->async_write(msg, std::bind(&pipe_client::handle_write,
			this,
			std::placeholders::_1,
			std::placeholders::_2
			));
	}
	else
	{
		return false;
	}
	return true;
}
void pipe_client::handle_write(std::error_code ec, size_t size)
{
	if (ec)
	{
		printf("write failed!\n");
		close();
		return;
	}
	else
	{
		printf("write ok %d!\n", size);
		return;
	}
}