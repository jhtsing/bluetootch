#ifndef __NO_COPY_DEF_H__
#define __NO_COPY_DEF_H__ 
#include <boost/cstdint.hpp>

namespace bluetooch
{
	template <int major,int minor>
	class socket_init
	{
	public:
		socket_init()
		{
			WSADATA wsa;
			result_ = ::WSASocket(MAKEWORD(major ,minor) , &wsa);
		}
		~socket_init()
		{
			::WSACleanup();
		}
		static socket_init& instance()
		{
			return instance_;
		}
	private:
		int result_;
		static socket_init instance_;
	};
	template <int major , int minor>
	socket_init<major , minor>& socket_init<major , minor>::instance_;
}
#endif