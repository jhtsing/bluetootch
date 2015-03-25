#ifndef __IP_ADDRESS_H__
#define __IP_ADDRESS_H__
#include <cstdint>
#include <string>
#include <winsock2.h>
#include <mswsock.h> 

namespace network
{
	class ip_address
	{
	public:
		ip_address(std::uint32_t addr) :
			address_(addr)
		{
		}
		~ip_address()
		{
		}
		const std::uint32_t address() const
		{
			return address_;
		}
		bool operator==(const ip_address &ipaddr)
		{
			return address_ == ipaddr.address_;
		}

		bool operator!=(const ip_address &ipaddr)
		{
			return address_ != ipaddr.address_;
		} 
	private:
		std::uint32_t address_; 
	}; 
	inline static ip_address parse(const std::string &str)
	{
		std::uint32_t address = ::ntohl(::inet_addr(str.c_str()));
		return ip_address(address);
	}
	inline static std::string parse(const ip_address &addr)
	{
		in_addr tmp = { 0 };
		tmp.s_addr = addr.address();
		char *p = ::inet_ntoa(tmp);
		return p;
	}

}

#endif 