#include "stdafx.h"
#include "utils.h"
namespace utils {

	class ScoketInit {
	public:
		ScoketInit() :
			is_ok_(false) {
			WSADATA wsaData;
			error_code_ = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (error_code_ == 0) {
				is_ok_ = true;
			}
		}
		~ScoketInit() {
			if (is_ok_) {
				WSACleanup();
			}
		}
		bool IsSucceed() const {
			return is_ok_;
		}
	private:
		bool is_ok_;
		int error_code_;
	};
	bool GetLocalIP(std::string& ip_addr)
	{
		//1.初始化wsa  
		ScoketInit init;
		if (!init.IsSucceed())
		{
			return false;
		}
		//2.获取主机名  
		char hostname[256];
		int ret = gethostname(hostname, sizeof(hostname));
		if (ret == SOCKET_ERROR) {
			return false;
		}
		//3.获取主机ip  
		HOSTENT* host = gethostbyname(hostname);
		if (host == NULL) {
			return false;
		}
		//4.转化为char*并拷贝返回
		char ip[32] = { 0 };
		strcpy_s(ip, 32, inet_ntoa(*(in_addr*)*host->h_addr_list));
		ip_addr = ip;
		return true;
	}
	void Split(const std::string& s, const std::string& delim, std::vector<std::string >& vec_strings)
	{
		size_t last = 0;
		size_t index = s.find_first_of(delim, last);
		while (index != std::string::npos)
		{
			vec_strings.push_back(s.substr(last, index - last));
			last = index + 1;
			index = s.find_first_of(delim, last);
		}
		if (index - last>0)
		{
			vec_strings.push_back(s.substr(last, index - last));
		}
	} 

	void LogOutput(const char *formats, ...) {
		va_list va;
		char buff[1024] = {0};
		va_start(va, formats);
		vsprintf_s(buff, 1023 ,formats, va);
		OutputDebugStringA(buff);
		va_end(va);
	}
}

