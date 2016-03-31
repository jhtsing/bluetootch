#ifndef __COMMAND_PARSE_H__
#define __COMMAND_PARSE_H__
#include <list>
#include <vector>
#include "singleton.h"
#include "control_center.h"

//注意：当字符串为空时，也会返回一个空字符串
void split(const std::string& s, const std::string& delim, std::vector<std::string >& ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		ret.push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last>0)
	{
		ret.push_back(s.substr(last, index - last));
	}
}

class CommandParser : public Singleton<CommandParser>
{
public:
	ENABLE_SINGLETON;
	void Parse(std::string& recv_buf){
		buf_.append(recv_buf);
		std::vector<std::string> commands;
		split(recv_buf, ";", commands);
		if (buf_[buf_.size() - 1] != ';'){
			buf_ = commands[commands.size() - 1];
		}
	}
	void RegeditCallBack(const std::shared_ptr<ControlCenter>& control_center){

		control_center_ = control_center;
	}
	void UnRegeditCallBack(){
		if (control_center_.lock()){
			control_center_.reset();
		}
	}
private:
	void doCallBack(const std::vector<std::string>& commands){
		std::shared_ptr<ControlCenter> cc(control_center_.lock());
		if (cc){
		}
	}
	CommandParser();
	std::string buf_;
	std::list<std::string> queue_;
	std::weak_ptr<ControlCenter> control_center_;
};
#endif 