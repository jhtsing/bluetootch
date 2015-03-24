#ifndef __SOCKET_FUNCTION_H__
#define __SOCKET_FUNCTION_H__

namespace base
{
	class noncopyable
	{
	protected:
		noncopyable() {}
		~noncopyable() {}
	private:
		noncopyable(const noncopyable&);
		const noncopyable& operator=(const noncopyable&);
	};
}





#endif  