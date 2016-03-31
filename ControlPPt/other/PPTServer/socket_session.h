#pragma once
#include <WinSock2.h>
#include <thread>
#include <memory>
#include <atomic>
class SocketSession : public std::enable_shared_from_this<SocketSession> {
public:
	SocketSession(const SOCKET& socket , std::weak_ptr<ControlCenter> control_center) :
	socket_(socket){
		is_stop_ = false;
	}
	~SocketSession() {
		Stop();
	}
	bool Start() {
		if (thread_){
			thread_ = std::make_shared<std::thread>(std::thread(std::bind(&SocketSession::Run, shared_from_this())));
		}
		if (!thread_){
			return true;
		}
		return false;
	}
	void Stop() {
		is_stop_ = true;
		if(socket_!=-1){
			::shutdown(socket_, SB_BOTH);
			closesocket(socket_);
			socket_ = -1;
		}
		if(thread_){
			thread_->join();
		}
	}
private:
	void Run(){
		while (!is_stop_){
			// receive
			char recvBuf[100];
			recv(socket_, recvBuf, 100, 0);
			printf("%s", recvBuf);
		}
	}
	SOCKET socket_;
	std::atomic_bool is_stop_;
	std::shared_ptr<std::thread> thread_;
};
