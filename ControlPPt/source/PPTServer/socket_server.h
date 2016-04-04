#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

#include <WinSock2.h>
#include <map>
#include <stdio.h>
#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include "socket_session.h"
#pragma comment(lib, "ws2_32.lib") 
class ControlCenter;
class SocketServer :
	public std::enable_shared_from_this<SocketServer> {
public:
	SocketServer(const std::string& ip_addr, int port ,std::weak_ptr<ControlCenter> cc);
	~SocketServer();
	bool Start();
	void Stop();
	void Run();
private:
	void NotifyClose();
	void NotifyNewSocket(SOCKET id);
	size_t port_;
	std::string addr_;
	SOCKET server_scoket_;
	std::atomic_bool is_stop_;
	std::mutex mutex_;
	std::shared_ptr<std::thread> accept_thread_;
	std::weak_ptr<ControlCenter> control_server_;
};
#endif