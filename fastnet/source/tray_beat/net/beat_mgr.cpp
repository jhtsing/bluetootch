#include "beat_mgr.h"
#include <iostream>
#include "include\log.h"

const std::string ts = "test";
beat_mgr::beat_mgr():
io_service_(1)
{
}
beat_mgr::~beat_mgr()
{
	uninit();
}
bool beat_mgr::init()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (!timer_)
	{
		timer_.reset(new basic_timer_t());
		if (!timer_)
		{
			return false;
		}
		if (!timer_->start())
		{
			return false;
		}
	}
	if (!request_)
	{
		request_.reset(new net_request(io_service_ , "127.0.0.1" , 8001));
		if (!request_)
		{
			return false;
		}
		if (!request_->init())
		{
			return false;
		}
	}
	return true;
}
void beat_mgr::uninit()
{
	stop();
}
bool beat_mgr::start()
{
	std::unique_lock<std::mutex> lock(mutex_); 
	if (timer_)
	{
		timer_->async_wait(std::bind(&beat_mgr::handle_beat,
			shared_from_this()),
			std::chrono::milliseconds(2000)
			);
		return true;
	}
	else
	{
		return false;
	}
}
void beat_mgr::async_read()
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (request_)
	{
		request_->async_read(std::bind(&beat_mgr::handle_read,
			shared_from_this(),
			std::placeholders::_1,
			std::placeholders::_2));
	}
}
void beat_mgr::handle_write(std::error_code ec, std::uint32_t size)
{
	if (!ec)
	{
		base::log_out_a("handle wirte ok size: %d\n" , size);
		async_read();
	}
	else
	{
		base::log_out_a("handle wirte failed\n");
	}
} 
void beat_mgr::handle_read(message msg, std::error_code ec)
{
	if (!ec)
	{
		// handle command
		char buf[1024] = {0};
		memcpy(buf, msg.data(), msg.data_size());
		base::log_out_a("recv data %s\n", buf);
	}
}
void beat_mgr::async_write()
{
	std::unique_lock<std::mutex> lock(mutex_); 
	if (request_)
	{
		request_->async_write(
			ts.c_str(),
			ts.size(),
			std::bind(&beat_mgr::handle_write,
			shared_from_this(), 
			std::placeholders::_1,
			std::placeholders::_2
			));
	}
}
void beat_mgr::handle_beat()
{
	base::log_out_a("beat ......!\n");
	async_write();
}
void beat_mgr::stop()
{
	std::unique_lock<std::mutex> lock(mutex_); 
	if (timer_)
	{
		timer_->cancel();
	}
	if (request_)
	{
		request_->close();
	}

}
