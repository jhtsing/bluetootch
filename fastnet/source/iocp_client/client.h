#pragma once
#include <memory>
#include "include\channel.h"
using namespace network;
class clientmgr
{
public:
	clientmgr();
	~clientmgr();
	bool init();
	void uninit();
	bool connect();
	bool send(const char* buf, int len);
	void close();
private:
	void handle_write(std::error_code ec, std::uint32_t size);
	channel_ptr chl_;
	service::iocp_impl io_service_;
};

