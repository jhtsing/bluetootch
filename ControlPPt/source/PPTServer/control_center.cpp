#include "stdafx.h"
#include "utils.h"
#include "control_center.h"
#include "command_parser.h"
ControlCenter::ControlCenter() {

}
ControlCenter::~ControlCenter() {

}
bool ControlCenter::Init() {
	std::unique_lock<std::mutex> lock(mutex_);
	if(!task_poll_){
		task_poll_.reset(new TaskPoll(1));
	}
	if (!task_poll_) {
		return false;
	}
	//CommandParser::Instance()->RegeditCallBack(shared_from_this());
	std::string ip_addr;
	if (!utils::GetLocalIP(ip_addr)) {
		printf("get local ip failed!\n");
		return false;
	}
	if (!socket_server_) {
		socket_server_.reset(new SocketServer("192.168.1.13" , 8019 ,shared_from_this()));
	}
	if (!socket_server_){
		return false;
	}
	return true;
}
bool ControlCenter::Start() {
	std::unique_lock<std::mutex> lock(mutex_);
	bool bret = false;
	CommandParser::Instance()->RegeditCallBack(shared_from_this());
	if (task_poll_) {
		bret = task_poll_->Start();
		if (!bret) {
			return bret;
		}
	}
	else {
		return false;
	}
	if (socket_server_) {
		bret = socket_server_->Start();
		if (!bret) {
			return bret;
		}
	}
	else {
		return false;
	}
	return true;
}
void ControlCenter::Stop() {
	std::unique_lock<std::mutex> lock(mutex_);
	if (task_poll_) {
		task_poll_->Stop();
	}
	if (socket_server_) {
		socket_server_->Stop();
	}
	CommandParser::Instance()->UnRegeditCallBack();
}
void ControlCenter::UnInit() {
	std::unique_lock<std::mutex> lock(mutex_);
	if (task_poll_) {
		task_poll_.reset();
	}
	if (socket_server_) {
		socket_server_.reset();
	}
}
void ControlCenter::DoTask(const std::string& command) {
	if (command.size()==0) {
		return;
	}
	if (command == "setfront") {
		simu_key_.SetWindowsWnd(_T("PPTFrameClass"));
	}
	else if (command == "pre") {
		simu_key_.SetLeft();
	}
	else if (command == "post") {
		simu_key_.SetRight();

	}else if (command == "start_head") {
		simu_key_.SetWindowsWnd(L"PPTFrameClass");
		simu_key_.SetF5();
	}else if (command == "start_current") {
		simu_key_.SetWindowsWnd(L"PPTFrameClass");
		simu_key_.SetShiltF5();

	}else if (command == "stop") {
		simu_key_.SetEsc();
	}
	else {
		MessageBox(NULL, _T("ÅªÉ¶ßÖ"), _T("ÑÏÖØ¾¯¸æ"), MB_OK);
	}
}
void ControlCenter::OnHandleSocketServerClose(std::shared_ptr<SocketServer>& socket_server) {
	if (task_poll_&&socket_server) {
		task_poll_->Post(std::bind(&SocketServer::Stop, socket_server));
	}
}
void ControlCenter::OnHandleSocketSessionClose(std::shared_ptr<SocketSession>& socket_session) {
	if (task_poll_&&socket_session) {
		task_poll_->Post(std::bind(&SocketSession::Stop, socket_session));
	}
}
void ControlCenter::OnHandleAccept(SOCKET id) {

	std::shared_ptr<SocketSession> _s(new SocketSession(id ,shared_from_this()));
	if (_s) {
		if (_s->Start()) {
			printf("OnHandleAccept input!\n");
		}
		else{
			_s->Stop();
		}
	}
}
void ControlCenter::OnHandleCommand(std::vector<std::string> commands) {
	std::unique_lock<std::mutex> lock(mutex_);
	if (task_poll_) {
		for (int i = 0; i < commands.size(); i++) {
			task_poll_->Post(std::bind(&ControlCenter::DoTask, shared_from_this(), commands[i]));
		}
	}
}
