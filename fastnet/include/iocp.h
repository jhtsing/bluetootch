#ifndef __IOCP_H__
#define __IOCP_H__
#include <Windows.h>
#include <assert.h>
namespace service
{
	class iocp
	{
	public:
		iocp() :
		handle_(NULL)
		{
		}
		~iocp()
		{
			close();
		}
		void close()
		{
			if (handle_)
			{
				CloseHandle(handle_);
				handle_ = NULL;
			}
		}
		bool create(int max_threads)
		{
			handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, max_threads);
			if (NULL == handle_)
			{
				return false;
			}
			return true;
		}
		bool bind(HANDLE handle, ULONG_PTR completion_key)
		{
			assert(handle_ != NULL);
			HANDLE _handle = CreateIoCompletionPort(handle, handle_, completion_key, 0);
			assert(_handle == handle_);
			return (handle_ == _handle);
		}
		bool get_status(DWORD& number_bytes, ULONG_PTR& lpCompletionKey, OVERLAPPED **overlapped, DWORD wait_mill_secs)
		{
			assert(handle_ != NULL);
			BOOL bret = GetQueuedCompletionStatus(handle_, &number_bytes, &lpCompletionKey, overlapped, wait_mill_secs);
			return (bret == TRUE);
		}
		bool get_status_ex(OVERLAPPED_ENTRY over_lapped[], DWORD overlapped_number, DWORD& ret_number, DWORD wait_mill_secs = INFINITE)
		{
			assert(handle_ != NULL);
			BOOL bret = GetQueuedCompletionStatusEx(handle_, over_lapped, overlapped_number, &ret_number, wait_mill_secs, TRUE);
			return (bret == TRUE);
		}
		bool post(OVERLAPPED* over_lapped, DWORD bytes_post = 0, ULONG_PTR completion_key = 0)
		{
			assert(handle_ != NULL);
			BOOL bret = PostQueuedCompletionStatus(handle_, bytes_post, completion_key, over_lapped);
			return (bret == TRUE);
		}
	private:
		HANDLE handle_;
	};
}




#endif 