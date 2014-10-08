#ifndef __DISPATCH_H__
#define __DISPATCH_H__
#include <boost/scoped_ptr.hpp>
#include "common/nocopy.hpp"
namespace bluetooch
{
	class dispatchor:public nocopy
	{
	 public:
		explicit dispatchor(size_t num_thread);
		~dispatchor(void);
		void bind(void *);
		// 向完成端口投递请求
		template<typename HandlerT>
		void post(HandlerT& handle);
		// 停止服务
		void stop();	     
	 private:
		struct impl;
		boost::scoped_ptr<impl> impl_;
	}; 
}
#endif

