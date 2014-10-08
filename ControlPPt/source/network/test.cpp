// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "dispatcher/call_back_type.h"

int test()
{
	if (true)
	{
		throw ("error");
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		test();
	}
	catch (...)
	{
		printf("error");
	}
	getchar();
	return 0;
}

