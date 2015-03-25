#include <winsock2.h>  
#include <MSTcpIP.h> 



namespace network
{

	template<int _Level, int _Name>
	class boolean_t
	{
	private:
		int value_;
	public:
		explicit boolean_t(bool val)
			: value_(val ? 1 : 0)
		{}
		boolean_t &operator=(const boolean_t &rhs)
		{
			value_ = rhs.value_;
			return *this;
		}
		boolean_t &operator=(bool val)
		{
			value_ = val ? 1 : 0;
		}

	public:
		bool value() const
		{
			return !!value_;
		}

		// bool 转换
		operator bool() const
		{
			return !!value_;
		}

		bool operator!() const
		{
			return !value_;
		}

		// 获取Socket leval选项值
		int level() const
		{
			return _Level;
		}

		// 获取Socket name
		int name() const
		{
			return _Name;
		}

		// 获取Socket值
		char *data()
		{
			return reinterpret_cast<char *>(&value_);
		}
		const char *data() const
		{
			return reinterpret_cast<const char *>(&value_);
		}

		// 获取值大小
		size_t size() const
		{
			return sizeof(value_);
		}


		// 设置boolean的大小
		void resize(size_t nSize)
		{
			// 在某些平台，getsockopt返回一个sizeof(bool)--1字节。	
			// 
			switch (nSize)
			{
			case sizeof(char) :
				value_ = *reinterpret_cast<char *>(&value_) ? 1 : 0;
				break;
			case sizeof(value_) :
				break;
			default:
				throw exception::exception_base("bool socket opetion resize");
			}
		}
	};
 

	template < size_t _CMD >
	class socket_control_t;
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
	typedef boolean_t<SOL_SOCKET, SO_REUSEADDR> reuse_addr; 
	typedef socket_control_t<SIO_GET_EXTENSION_FUNCTION_POINTER> extension_function;
}
