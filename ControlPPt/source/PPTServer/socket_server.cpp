#include "stdafx.h"
#include "socket_server.h"
#include "socket_session.h"
#include "control_center.h"
SocketServer::SocketServer(const std::string& ip_addr, int port, std::weak_ptr<ControlCenter> cc):
addr_(ip_addr),
port_(port),
control_server_(cc){
	is_stop_ = false;
}
SocketServer::~SocketServer() {
	Stop();
}
bool SocketServer::Start() {

	WORD wVerisonRequested = MAKEWORD(1, 1);
	WSADATA wsaData;
	int err = WSAStartup(wVerisonRequested, &wsaData);
	if (err != 0){
		return false;
	}
	server_scoket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (server_scoket_ == -1){
		return false;
	}
	// bind socket
	SOCKADDR_IN addr_server;
	addr_server.sin_addr.S_un.S_addr = inet_addr(addr_.c_str());
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(port_);
	if (bind(server_scoket_, (SOCKADDR *)&addr_server, sizeof(addr_server)) != 0){
		return false;
	}
	// listen
	if (listen(server_scoket_, 5) != 0){
		return false;
	}
	if (!accept_thread_){
		accept_thread_ = std::make_shared<std::thread>(std::thread(std::bind(&SocketServer::Run, shared_from_this())));
	}
	if (nullptr == accept_thread_){
		return false;
	}
	return true;
}
void SocketServer::Stop() {
	is_stop_ = true;
	if (server_scoket_ != -1){
		::shutdown(server_scoket_, SD_BOTH);
		closesocket(server_scoket_);
		server_scoket_ = -1;
	}
	if (accept_thread_->joinable()){
		accept_thread_->join();
	}
}
void SocketServer::NotifyNewSocket(SOCKET id) {
	std::shared_ptr<ControlCenter> cc(control_server_.lock());
	if (cc) {
		cc->OnHandleAccept(id);
	}
}
void SocketServer::NotifyClose() {
	std::shared_ptr<ControlCenter> cc(control_server_.lock());
	if (cc) {
		cc->OnHandleSocketServerClose(shared_from_this());
	}
}
void SocketServer::Run() {
	while (!is_stop_){
		// accept
		SOCKADDR_IN addrClient;
		int len = sizeof(addrClient);
		SOCKET new_socket = accept(server_scoket_, (SOCKADDR *)&addrClient, &len);
		if (new_socket != INVALID_SOCKET){
			NotifyNewSocket(new_socket);
		}else{
			printf("accept failed occur %d!\n", WSAGetLastError());
			break;
		}
	}
}

