#ifndef __SESSION_H__
#define __SESSION_H__ 
#include "include\p_channel.h"
#include <memory>
using namespace named_pipe;
class pipe_session :
	public std::enable_shared_from_this<pipe_session>
{
public:
	pipe_session(p_channel::ptr& ptr);
	~pipe_session();
	bool start();
	void stop();
private:
	void async_read();
	void handle_read(const std::shared_ptr<pipe_message>, std::error_code);
	p_channel::ptr ptr_;
};

#endif 
