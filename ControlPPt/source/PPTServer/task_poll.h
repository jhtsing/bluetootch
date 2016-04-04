#ifndef __TASK_POLL_H__
#define __TASK_POLL_H__

#include <list>
#include <atomic>
#include <mutex>
#include <memory>
#include <vector>
#include <thread>
#include <functional>
class TaskPoll :
	public std::enable_shared_from_this<TaskPoll> {
public:
	typedef std::function<void ()> FunctionType;
	typedef std::vector<std::shared_ptr<std::thread> > ThreadListType;
	TaskPoll(int thread_size);
	~TaskPoll();
	bool Start();
	void Stop();
	void Post(const FunctionType& task);
private:
	void Run();
	int thread_size_;
	std::atomic_bool is_stop_;
	std::mutex mutex_;
	std::condition_variable cond_;
	ThreadListType threads_;
	std::list<FunctionType> tasks_;
};
#endif