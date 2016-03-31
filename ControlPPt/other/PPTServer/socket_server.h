#pragma once

#include <WinSock2.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <atomic>
#pragma comment(lib, "ws2_32.lib") 

class ControlCenter;
class SocketServer{

public:
	SocketServer(const std::string& ip_addr, int port );
	~SocketServer();
	bool Start();
	void Stop();
	void Run();
private:
	size_t port_;
	std::string addr_;
	std::atomic_bool is_stop_;
	std::weak_ptr<ControlCenter> control_center_;
	std::shared_ptr<std::thread> accept_thread_;
	SOCKET server_scoket_;
};