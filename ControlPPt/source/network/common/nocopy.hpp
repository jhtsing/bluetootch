#ifndef __NO_COPY_DEF_H__
#define __NO_COPY_DEF_H__ 
#include <boost/cstdint.hpp>

namespace bluetooch
{
	class nocopy
	{
	protected:
		nocopy(){}
		~nocopy(){}
	private:
		nocopy(const nocopy&);
		nocopy& operator=(const nocopy&);
	};
}
#endif