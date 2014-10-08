#ifndef __BULE_NET_IOCP_HPP
#define __BULE_NET_IOCP_HPP
#include <Windows.h>
#include <cassert>
#include <boost/noncopyable.hpp>
#include "common/nocopy.hpp"
namespace bluetooch{
	// class iocp_handle
	class iocp_handle:
	public nocopy
	{
	public:
		iocp_handle()
		:iocp_(NULL)
		{}
		~iocp_handle()
		{
			close();
		}
	public:
		bool is_open() const
		{
			return iocp_ != NULL;
		}
		void close()
		{
			if(NULL!=iocp_)
			{
				BOOL bRes = ::CloseHandle(iocp_);
				iocp_ = 0;
			}
		}
		bool create(size_t nMaxConcurrency)
		{
			iocp_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, nMaxConcurrency);
			assert(iocp_ != NULL);
			return iocp_ != NULL;
		}
		bool associate_device(HANDLE hDevice, ULONG_PTR ulCompKey)
		{
			assert(iocp_ != NULL);
			return ::CreateIoCompletionPort(hDevice, iocp_, ulCompKey, 0) == iocp_;
		}
		bool post_status(ULONG_PTR ulCompKey, DWORD dwNumBytes = 0, OVERLAPPED *pOver = 0)
		{
			BOOL bOk = ::PostQueuedCompletionStatus(iocp_, dwNumBytes, ulCompKey, pOver);
			return bOk == TRUE;
		}
		bool get_status(ULONG_PTR *pCompKey, PDWORD pdwNumBytes, OVERLAPPED **pOver, DWORD dwMilliseconds = INFINITE)
		{
			return TRUE == ::GetQueuedCompletionStatus(iocp_, pdwNumBytes, pCompKey, pOver, dwMilliseconds);
		}
		operator HANDLE()
		{
			return iocp_;
		}
		operator const HANDLE() const
		{
			return iocp_;
		}
	private:
		HANDLE iocp_;
	};
}
#endif
