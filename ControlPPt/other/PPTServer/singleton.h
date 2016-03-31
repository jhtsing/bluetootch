#ifndef __SINGLETON_H__
#define __SINGLETON_H__
#include <memory>
#include <atomic>
#include <mutex>

template <typename T>
class Singleton {
public:
	static T * Instance()
	{
		T * tmp = instance_.load(std::memory_order_consume);
		if (!tmp) {
			std::mutex::scoped_lock guard(mutex_);
			tmp = instance_.load(std::memory_order_consume);
			if (!tmp) {
				tmp = new T;
				instance_.store(tmp, std::memory_order_release);
			}
		}
		return tmp;
	}
protected:
	Singleton(){} 
private:
	Singleton(const Singleton& rhs);
	void operator =(const Singleton&);

private:
	static std::atomic<T *> instance_;
	static std::mutex mutex_;
};

template <typename T>
std::atomic<T *> Singleton<T>::instance_(0);

template <typename T>
std::mutex Singleton<T>::mutex_; 


#ifndef ENABLE_SINGLETON
#	define  ENABLE_SINGLETON \
	template<typename T> friend class Singleton;
#endif 


#endif 