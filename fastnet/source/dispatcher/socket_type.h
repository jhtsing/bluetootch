#include <winsock.h>  
#include <MSTcpIP.h> 

typedef SOCKET socket_native;

namespace network
{
	template <>
	class socket_control_t<SIO_GET_EXTENSION_FUNCTION_POINTER>
	{
	public:
		socket_control_t(const GUID& guid, LPVOID func):
		guid_(guid),
		func_(func)
		{

		}
		u_long cmd() const
		{
			return SIO_GET_EXTENSION_FUNCTION_POINTER;
		}

		void *in_buffer()
		{
			return &guid_;
		}

		size_t in_buffer_size() const
		{
			return sizeof(guid_);
		}

		LPVOID out_buffer()
		{
			return func_;
		}

		size_t out_buffer_size() const
		{
			return sizeof(func_);
		}
	private:
		GUID guid_;
		LPVOID func_;
	};

}
