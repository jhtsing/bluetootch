#include "stdafx.h"
#include "task_poll.h"
#include <Windows.h>

TaskPoll::TaskPoll(int thread_size):thread_size_(thread_size){
	is_stop_ = false;
}
TaskPoll::~TaskPoll() {

}
bool TaskPoll::Start() {
	std::unique_lock<std::mutex> lock(mutex_);
	std::unique_ptr<std::thread> t_;
	for (int i = 0; i < thread_size_; i++){
		try {
			t_.reset(new std::thread(std::bind(&TaskPoll::Run, shared_from_this())));
			if (t_){
				threads_.push_back(std::shared_ptr<std::thread>(t_.release()));
			}
		}
		catch (...){
			printf("run file failed!\n");
		}
	}
	return (thread_size_ == threads_.size());
}
void TaskPoll::Stop() {
	is_stop_ = false;
	cond_.notify_all();
	std::unique_lock<std::mutex> lock(mutex_);
	tasks_.clear();
	ThreadListType _threads;
	_threads.swap(threads_);
	lock.unlock();
	for (int i = 0; i < _threads.size(); i++) {
		if (_threads[i]->get_id() == std::this_thread::get_id()){
			printf("kill self!\n");
		}
		else {
			if (_threads[i]->joinable()) {
				_threads[i]->join();
				_threads[i].reset();
			}
		}
	}
}
void TaskPoll::Post(const FunctionType& task) {
	std::unique_lock<std::mutex> lock(mutex_);
	tasks_.push_back(task);
	cond_.notify_all();
}
void TaskPoll::Run() {
	while (!is_stop_) {
		std::unique_lock<std::mutex> lock(mutex_);
		while (tasks_.empty()&&!is_stop_) {
			cond_.wait(lock);
		}
		if (is_stop_){
			break;
		}
		FunctionType task = tasks_.front();
		tasks_.pop_front();
		lock.unlock();
		task();
	}
}