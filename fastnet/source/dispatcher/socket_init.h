#ifndef __SOCKET_INIT_H__
#define __SOCKET_INIT_H__
#include <windows.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

namespace network
{
	template <int marjor = 2, int minor = 0>
	class socket_init
	{
	public:
		socket_init():
		error_code_(0)
		{
			WSADATA wsa_data;
			WORD version = MAKEWORD(marjor, minor);
			error_code_ = ::WSAStartup(version, &wsa_data);
		}
		int GetError()
		{
			return error_code_;
		}
		~socket_init()
		{
			::WSAClean();
		}
	private:
		int error_code_;
	};
}

#endif  