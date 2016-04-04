#ifndef __COMMAND_PARSER_H__
#define __COMMAND_PARSER_H__
#include <list>
#include <vector>
#include "utils.h"
#include "singleton.h"
#include "control_center.h"
class ControlCenter;
class CommandParser : 
	public Singleton<CommandParser>
{
public:
	ENABLE_SINGLETON;
	~CommandParser() {}
	void Parse(const std::string& recv_buf);
	void RegeditCallBack(const std::shared_ptr<ControlCenter>& control_center);
	void UnRegeditCallBack();
private:
	void doCallBack(const std::vector<std::string>& commands);
	CommandParser() {}
	std::string buf_;
	std::weak_ptr<ControlCenter> control_center_;
};
#endif 