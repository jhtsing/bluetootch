#ifndef __UTILS_H__
#define __UTILS_H__
#include <string>
#include <vector>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
namespace utils {
	bool GetLocalIP(std::string& ip_addr);
	void Split(const std::string& s, const std::string& delim, std::vector<std::string >& ret);
	void LogOutput(const char *formats , ...);
}
#endif

