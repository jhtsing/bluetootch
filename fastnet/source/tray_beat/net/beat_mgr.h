#ifndef __BEAT_MGR_H__
#define __BEAT_MGR_H__
#include <memory>
#include "include\nocopyable.h"
#include "basic_timer.hpp"
#include "request.h"
#include "include\message.h"
class beat_mgr :
	public base::noncopyable,
	public std::enable_shared_from_this<beat_mgr>
{
public:
	beat_mgr();
	~beat_mgr();
	bool init();
	void uninit();
	bool start();
	void stop();
	void handle_read(message msg, std::error_code ec);
	void handle_write(std::error_code ec, std::uint32_t size);
	void handle_beat();
	void async_read();
	void async_write();
private:
	std::mutex mutex_;
	request_ptr request_;
	timer_handle_ptr timer_;
	service::iocp_impl io_service_;
};
#endif 