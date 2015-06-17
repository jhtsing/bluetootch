#ifndef __PIPE_CLIENT_H__
#define __PIPE_CLIENT_H__
#include <memory>
#include <string>
#include "include\p_channel.h"
#include "include\iocp_impl.h"
using namespace named_pipe;
class pipe_client
{
public:
	pipe_client();
	~pipe_client();
	bool init();
	bool connect(const std::string& name);
	bool send(const char* buf ,int size);
	void close();
private:
	void handle_write(std::error_code ec, size_t size);
	p_channel::ptr chl_;
	service::iocp_impl io_service_; 
};
#endif 