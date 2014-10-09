#ifndef __NO_COPY_DEF_H__
#define __NO_COPY_DEF_H__ 
#include <boost/cstdint.hpp>

namespace bluetooch
{
	class socket_handle
	{
	public:
		socket_handle(){}
		~socket_handle(){}
		bool is_open();
		bool open(int family ,int nType,int nProtocol);
		bool bind(int family ,const ip_address& addr , size_t port);
		bool listen(int max);
		void connect(int family, const ip_address& addr , size_t port);
		void disconnect(int shut_type, bool is_reuse);
		void close();
	private:
		SOCKET socket_;
	};
}
#endif