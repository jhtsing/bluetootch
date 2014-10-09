#ifndef __NO_COPY_DEF_H__
#define __NO_COPY_DEF_H__ 
#include <boost/cstdint.hpp>

namespace bluetooch
{
	typedef boost::int32_t int32;
	class ip_address
	{
	 public:
		 ip_address(const int32& addr):
		   addr_(addr)
		{}
		~ip_address(){}
		int32 address()
		{
			return addr_;
		}
		bool operator==(const ip_address& rs)
		{
			return (rs.address()==addr_);
		}
		bool operator!=(const ip_address& rs)
		{
			return (rs.address()!= addr_);
		}
	private:
		static std::string address_to_string(const ip_address& addr)
		{
			in_addr _addr;
			_addr.s_addr = addr.address();
			char * p = ::inet_ntoa(tmp);
			return p;
		}
		static ip_address string_to_address(const std::string& addr)
		{
			int32 _addr = ::htonl(::inet_addr(addr.c_str()));
			return ip_address(_addr);
		}
		int32 addr_;
	};
}
#endif