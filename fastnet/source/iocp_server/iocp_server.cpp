// iocp_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "server_mgr.h"
#include "include\socket_init.h"
network::socket_init<2, 0> sock_init_;
int _tmain(int argc, _TCHAR* argv[])
{
	std::shared_ptr<server_mgr> mgr(new server_mgr());
	if (!mgr)
	{
		return 0;
	}
	bool bret = false;
	bret =mgr->init();
	bret = mgr->start();
	getchar();
	mgr->uninit();
	mgr->stop();
	return 0;
}

