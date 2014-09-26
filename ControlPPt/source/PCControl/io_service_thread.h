#ifndef __IO_SERVICE_H__
#define __IO_SERVICE_H__
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
class io_service_thread
{
public:
	virtual ~io_service_thread()
	{
		stop_thread();
	}
	boost::asio::io_service& get_io_service()
	{
		return io_service_;
	}
	void start_thread()
	{
		io_work_.reset(new boost::asio::io_service::work(io_service_));
		thread_=boost::thread(boost::bind(&io_service_thread::run,this));
	}
	void stop_thread()
	{
		io_work_.reset();
		io_service_.stop();
		thread_.join();
		io_service_.reset();
	}
private:
	void run()
	{
		boost::system::error_code ec;
		io_service_.run(ec);
	}
private:
	boost::asio::io_service io_service_;
	boost::scoped_ptr<boost::asio::io_service::work> io_work_;
	boost::thread thread_;
};
#endif 