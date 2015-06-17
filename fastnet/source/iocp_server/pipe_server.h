#ifndef __PIPE_SERVER_MGR_H__
#define __PIPE_SERVER_MGR_H__ 
#include "include\iocp_impl.h"
#include "include\p_channel.h"
#include "include\pipe_acceptor.h"
using namespace named_pipe;
class pipe_mgr 
{
public:
	pipe_mgr();
	~pipe_mgr();
	bool init();
	void uninit();
	bool start();
	void stop();
private:
	void handle_accept(p_channel::ptr chl , std::error_code ec);
	std::vector<char> buf_;
	service::iocp_impl io_service_;
	std::shared_ptr<p_acceptor> acceptor_;
};
 

#endif 