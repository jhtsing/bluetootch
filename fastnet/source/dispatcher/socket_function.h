#ifndef __SOCKET_FUNCTION_H__
#define __SOCKET_FUNCTION_H__
#include "socket_type.h"
#include <MSWSock.h>
namespace network 
{
	class socket_function
	{
	public:
		socket_function();
		~socket_function();
	public:
		// 提供唯一实例
		static socket_function &singleton();
		// 获取扩展API
		static void get_extension_function(SOCKET &sock, const GUID &guid, LPVOID pFunc);
		static void cancel_io(SOCKET sock);
	private:
		LPFN_TRANSMITFILE			TransmitFile;
		LPFN_ACCEPTEX				AcceptEx;
		LPFN_GETACCEPTEXSOCKADDRS	GetAcceptExSockaddrs;
		LPFN_TRANSMITPACKETS		TransmitPackets;
		LPFN_CONNECTEX				ConnectEx;
		LPFN_DISCONNECTEX			DisconnectEx;
		LPFN_WSARECVMSG				WSARecvMsg;

	};
}
#endif 