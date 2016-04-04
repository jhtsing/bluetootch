#ifndef __CONTROL_CENTER_H__
#define __CONTROL_CENTER_H__
#include <memory>
#include <mutex>
#include "task_poll.h"
#include "simulation_key.h"
#include "socket_server.h"
class SocketServer;
class SocketSession;
class ControlCenter : 
	public std::enable_shared_from_this<ControlCenter>
{
public:
	ControlCenter();
	~ControlCenter();
	bool Init();
	bool Start();
	void Stop();
	void UnInit();
	void OnHandleAccept(SOCKET id );
	void OnHandleSocketServerClose(std::shared_ptr<SocketServer>& socket_server);
	void OnHandleSocketSessionClose(std::shared_ptr<SocketSession>& socket_session);
	void OnHandleCommand(std::vector<std::string> commands);
private:
	void DoTask(const std::string& commands);
	std::mutex mutex_;
	SimulationKey simu_key_;
	std::shared_ptr<TaskPoll> task_poll_;
	std::shared_ptr<SocketServer> socket_server_;
};
#endif

