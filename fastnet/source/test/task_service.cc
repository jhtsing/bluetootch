#include "stdafx.h"
#include "task_service.h"
#include <functional>
#include <memory>

task_service::task_service():
session_(0),
start_thread_counts_(0)
{
	is_stop_ = false;
}
task_service::~task_service()
{
}
bool task_service::start_thread(std::uint16_t thread_count)
{
	assert(thread_count > 0);
	if (is_stop_)
	{
		return false;
	}
	try
	{
		session_++;
		std::uint16_t read_thread_counts = 0; 
		std::unique_lock<std::mutex> lock(start_stop_mutex_);
		threads_.reserve(thread_count);
		for (int i = 0; i < thread_count; i++)
		{
			try
			{
				threads_.push_back(std::thread([this]
				{
					run();
				}));
				read_thread_counts++;
			}
			catch (std::exception& ec)
			{
				break;
			}
		}
		while (start_thread_counts_ < read_thread_counts)
		{
			std::this_thread::yield();
		}
	}
	catch (std::exception& ec)
	{
		return false;
	}
	return true;
}
void task_service::_stop()
{
	if (is_stop_)
	{
		return;
	}
	is_stop_ = true;
	cond_.notify_all();
	for (int i = 0; i < threads_.size(); i++)
	{
		threads_[i].join();
	}
	std::unique_lock<std::mutex> lock(start_stop_mutex_);
	threads_.clear();
}
bool task_service::post(task_type task, int priority)
{
	std::unique_lock<std::mutex> lock(mutex_);
	tasks_.insert(TaskItem(0, priority, task));
	cond_.notify_one();
	return true;
}
void task_service::terminate_thread()
{
	_stop();
}
void task_service::run()
{
	start_thread_counts_++;
	while (!is_stop_)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		while (tasks_.empty())
		{
			cond_.wait(lock);
		}
		if (is_stop_)
		{
			tasks_.clear();
			break;
		}
		if (tasks_.size())
		{
			task_set_type::iterator itor = tasks_.begin();
			if (itor == tasks_.end())
			{
				continue;
			}
			TaskItem item = *itor;
			task_type h;
			h.swap(item.task);
			tasks_.erase(itor);
			lock.unlock();
			h();
		}
	}
	start_thread_counts_--;
}
