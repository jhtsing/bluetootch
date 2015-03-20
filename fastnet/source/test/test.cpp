// test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "include\iocp_impl.h"

void handler(const std::error_code& ec, std::uint32_t size)
{
	if (ec)
	{
		printf("TID %d failed handler max size!\n" , GetCurrentThreadId());
	}
	else
	{
		printf("TID %d OK handler max size!\n", GetCurrentThreadId());
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	printf("Main TID %d!\n", GetCurrentThreadId()); 
	iocp_impl impl_(1);

	impl_.post(handler);
	impl_.post(handler);
	impl_.post(handler);



	getchar();
	return 0;
}

