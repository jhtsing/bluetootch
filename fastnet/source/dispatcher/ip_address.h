#ifndef __IP_ADDRESS_H__
#define __IP_ADDRESS_H__
#include <cstdint>
#include <string>
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
		static ip_address parse(const std::string &str);
		static std::string parse(const ip_address &addr); 
	private:
		std::uint32_t address_; 
	}; 
}

#endif 