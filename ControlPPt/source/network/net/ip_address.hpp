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


		int32 addr_;
	};
}
#endif