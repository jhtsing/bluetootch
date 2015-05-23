#ifndef __SERVER_MGR_H__
#define __SERVER_MGR_H__
#include "include\iocp_impl.h"
#include "include\acceptor.h"
#include "include\socket_handle_t.h"
using namespace network;
class server_mgr
{
public:
	server_mgr();
	~server_mgr();
	bool init();
	void uninit();
	bool start();
	void stop();
private:

	void handle_accept(socket_handle_ptr ptr, std::error_code ec);
	service::iocp_impl io_service_;
	std::vector<char> buf_; 
	std::shared_ptr<network::socket_acceptor> acceptor_;
};

#endif 