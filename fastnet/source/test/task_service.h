#ifndef __TASK_SERVICE_H__
#define __TASK_SERVICE_H__
#include <vector>
#include <set>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>
#include <condition_variable>
class task_service
{
public:
	typedef std::function<void()> task_type;
	task_service();
	~task_service();
	bool start_thread(std::uint16_t thread_count);
	void terminate_thread();
	bool post(task_type task,int priority);
private:
	struct TaskItem
	{
		TaskItem(int session, int priority ,task_type _task):
		task_session(session),
		task_priority(priority),
		task(_task)
		{}
		int task_session;
		int task_priority;
		task_type task;
	};
	struct greater_priority
	{
		bool operator()(const TaskItem&e1, const TaskItem& e2)const
		{
			return e1.task_priority>e2.task_priority;
		}
	}; 
	void run();
	void _stop();
	typedef std::multiset<TaskItem, greater_priority> task_set_type;
	std::mutex mutex_;
	std::int32_t session_;
	std::atomic_bool is_stop_;
	std::mutex start_stop_mutex_;
	std::uint16_t start_thread_counts_;
	std::condition_variable cond_;
	task_set_type tasks_;
	std::vector<std::thread> threads_;
};

#endif 
