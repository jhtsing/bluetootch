// iocp_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <set>
#include <algorithm>
#include "server_mgr.h"
#include "pipe_server.h"
#include "include\socket_init.h"
network::socket_init<2, 0> sock_init_;

int test_net_mgr()
{
	std::shared_ptr<server_mgr> mgr(new server_mgr());
	if (!mgr)
	{
		return 0;
	}
	bool bret = false;
	bret = mgr->init();
	bret = mgr->start();
	getchar();
	mgr->uninit();
	mgr->stop(); 
	return 1;
}
int test_pipe_mgr()
{
	std::shared_ptr<pipe_mgr> mgr(new pipe_mgr());
	if (!mgr)
	{
		return 0;
	}
	bool bret = false;
	bret = mgr->init();
	bret = mgr->start();
	getchar();
	mgr->uninit();
	mgr->stop();
	return 1;
}
int _tmain(int argc, _TCHAR* argv[])
{
	test_net_mgr();
	return 0;
}

