#include "stdafx.h"
#include "socket_session.h"
#include "utils.h"
#include "command_parser.h"

SocketSession::SocketSession(const SOCKET& socket, std::shared_ptr<ControlCenter>& cc) :
	socket_(socket),
	control_center_(cc) 
{
	is_stop_ = false;
}
SocketSession::~SocketSession() {
		//Stop();
}
bool SocketSession::Start() {
	if (!thread_) {
		thread_ = std::make_shared<std::thread>(std::thread(std::bind(&SocketSession::Run, shared_from_this())));
	}
	if (!thread_) {
		return false;
	}
	return true;
}
void SocketSession::Stop() {
	utils::LogOutput("-----Stop Event Occur---!\n");
	is_stop_ = true;
	Close();
	if (thread_) {
		if (std::this_thread::get_id() == thread_->get_id()) {
			utils::LogOutput("-----Stop bymyself Occur---!\n");
			return;
		}
		if (thread_->joinable()) {
			thread_->join();
		}
	}
}
void SocketSession::Close() {
	if (socket_ != -1) {
		::shutdown(socket_, SB_BOTH);
		closesocket(socket_);
		socket_ = -1;
	}
}
void SocketSession::NotifyClose() {
	std::shared_ptr<ControlCenter> cc(control_center_.lock());
	if (cc) {
		cc->OnHandleSocketSessionClose(shared_from_this());
	}
}
void SocketSession::Run() {
		
	while (!is_stop_) {
			// receive
		char recvBuf[100] = { 0 };
		int iret = recv(socket_, recvBuf, 100, 0);
		if (iret > 0) {
			CommandParser::Instance()->Parse(recvBuf);
			utils::LogOutput("recv %s!\n", recvBuf);
		}
		else if (iret == 0) {
			utils::LogOutput("recv meet close session!\n");
				//g_tss_delete = shared_from_this();
			NotifyClose();
			break;
		}
		else {
			utils::LogOutput("recv meet error %d!\n", WSAGetLastError());
				//g_tss_delete = shared_from_this();
			NotifyClose();
			break;
		}
	}
}
