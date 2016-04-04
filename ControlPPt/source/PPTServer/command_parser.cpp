#include "stdafx.h"
#include "command_parser.h"
void CommandParser::Parse(const std::string& recv_buf) {
	buf_.append(recv_buf);
	std::vector<std::string> commands;
	utils::Split(recv_buf, ";", commands);
	if ((commands.size()>0) &&
		(buf_.size()>0) &&
		(buf_[buf_.size() - 1] != ';')) {
		buf_ = commands[commands.size() - 1];
	}
	doCallBack(commands);
}
void CommandParser::RegeditCallBack(const std::shared_ptr<ControlCenter>& control_center) {
	control_center_ = control_center;
}
void CommandParser::UnRegeditCallBack() {
	if (control_center_.lock()) {
		control_center_.reset();
	}
}
void CommandParser::doCallBack(const std::vector<std::string>& commands) {
	std::shared_ptr<ControlCenter> cc(control_center_.lock());
	if (cc) {
		cc->OnHandleCommand(commands);
	}
}

