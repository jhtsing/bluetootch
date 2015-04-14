#include "include\socket_function.h"
#include <mutex> 
#include <memory>
namespace network
{
	socket_function::socket_function()
	{
		static GUID guidTransmitFile = WSAID_TRANSMITFILE;
		static GUID guidAcceptEx = WSAID_ACCEPTEX;
		static GUID guidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
		static GUID guidTransmitPackets = WSAID_TRANSMITPACKETS;
		static GUID guidConnectEx = WSAID_CONNECTEX;
		static GUID guidDisconnectEx = WSAID_DISCONNECTEX;
		static GUID guidWSARecvMsg = WSAID_WSARECVMSG;

		SOCKET sck = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

		get_extension_function(sck, guidTransmitFile, &TransmitFile);
		get_extension_function(sck, guidAcceptEx, &AcceptEx);
		get_extension_function(sck, guidGetAcceptExSockaddrs, &GetAcceptExSockaddrs);
		get_extension_function(sck, guidTransmitPackets, &TransmitPackets);
		get_extension_function(sck, guidConnectEx, &ConnectEx);
		get_extension_function(sck, guidDisconnectEx, &DisconnectEx);
		get_extension_function(sck, guidWSARecvMsg, &WSARecvMsg);
		if (sck)
		{
			::closesocket(sck);
		}
	}
	socket_function::~socket_function()
	{
	}
	socket_function& socket_function::singleton()
	{
		static std::once_flag flag;
		static std::shared_ptr<socket_function> provider;

		std::call_once(flag, [](){ provider.reset(new socket_function); });
		return *provider;
	}
	// ªÒ»°¿©’πAPI
	void socket_function::get_extension_function(SOCKET &sock, const GUID &guid, LPVOID pFunc)
	{
		extension_function ext_func(guid, pFunc);
		DWORD dwRet = 0;
		if (0 != ::WSAIoctl(sock, ext_func.cmd(), ext_func.in_buffer(), ext_func.in_buffer_size(),
			ext_func.out_buffer(), ext_func.out_buffer_size(), &dwRet, 0, 0))
		{
			printf("error");
		}
	}
	void socket_function::cancel_io(SOCKET sock)
	{
		::CancelIoEx(reinterpret_cast<HANDLE>(sock), nullptr);
	}

}
