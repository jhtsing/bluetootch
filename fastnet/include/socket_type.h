#ifndef __SOCKET_TYPE_H__
#define __SOCKET_TYPE_H__

#include <cstdint>
#include <string>
#include <WinSock2.h>
#include <Windows.h>
#include <MSWSock.h>
// Link with ws2_32.lib

namespace network
{

#define MAX_SCOKADDR_BUFFER (16+sizeof(SOCKADDR))

	// ----------------------------------------------------------
	// 基于int设置socket属性的帮助类

	template<int _Level, int _Name>
	class integer_t
	{
	private:
		int value_;
	public:
		integer_t()
			: value_(0)
		{}
		explicit integer_t(int val)
			: value_(val)
		{}
		integer_t &operator=(const integer_t &rhs)
		{
			value_ = rhs.value_;
			return *this;
		}
		integer_t &operator=(int val)
		{
			value_ = val;
		}

	public:
		bool value() const
		{
			return !!value_;
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
			return (const char *)&value_;
		}

		// 获取值大小
		size_t size() const
		{
			return sizeof(value_);
		}


		// 设置int的大小
		void resize(size_t nSize)
		{
			if (nSize != sizeof(value_))
				printf("Integer socket option resize!\n");
		}
	};


	// -----------------------------------------------------------------
	// class Linger 选项设置帮助类

	// SO_LINGER用于控制当未发送的数据在套接字上排队时，一旦执行了closesocket后，该采取何种动作
	// 如果超出延时，则未发送和接受的数据都会丢弃，同时重设对方的连接
	// 也可以通过设置SO_DONTLINGER来设置

	template< >
	class integer_t<SOL_SOCKET, SO_LINGER>
	{
	private:
		::linger value_;
	public:
		integer_t()
		{
			value_.l_linger = 0;	// option on/off
			value_.l_onoff = 0;	// linger time
		}
		integer_t(bool bOn, int nTime)
		{
			enabled(bOn);
			timeout(nTime);
		}


	public:
		// 打开或关闭linger选项
		void enabled(bool val)
		{
			value_.l_onoff = val ? 1 : 0;
		}

		bool enabled() const
		{
			return value_.l_onoff != 0;
		}

		void timeout(int val)
		{
			value_.l_linger = static_cast<u_short>(val);
		}
		int timeout() const
		{
			return static_cast<int>(value_.l_linger);
		}

		// 获取Socket leval选项值
		int level() const
		{
			return SOL_SOCKET;
		}

		// 获取Socket name
		int name() const
		{
			return SO_LINGER;
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

		// 设置int的大小
		void resize(size_t nSize)
		{
			if (nSize != sizeof(value_))
				printf("Linger socket option resize!\n");
		}
	};
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
	typedef integer_t<SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT> update_accept_context;
	typedef integer_t<SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT> update_connect_context;
}
#endif 