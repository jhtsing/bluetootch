// iocp_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "client.h"
#include "pipe_client.h"
#include "include\socket_init.h"
#include <memory>
#include <iostream>
const std::string send_buf = "test iocp network!";
network::socket_init<2,0> k_socket_init_;

const std::string k_pipe_name = "\\\\.\\pipe\\my_namedpipe"; 

int _tmain(int argc, _TCHAR* argv[])
{
	std::shared_ptr<pipe_client> cli_(new pipe_client());
	bool bret = cli_->init();
	bret = cli_->connect(k_pipe_name);
	if (!bret)
	{
		std::cout << "connect failed" << std::endl;
		return 0;
	}
	std::cout << "connect ok" << std::endl; 
	getchar();
	getchar();
	for (int i = 0; i < 100000; i++)
	{
		Sleep(10);
		bret = cli_->send(send_buf.c_str(), send_buf.length());
		if (!bret)
		{
			std::cout << "send ok" << std::endl;
			return 0;
		}
	}
	std::cout << "send finish" << std::endl;
	getchar();
	cli_->close();
	return 0;
}

