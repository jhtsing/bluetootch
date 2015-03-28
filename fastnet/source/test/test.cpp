// test.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>
#include "task_service.h"
void handler(int i)
{
	printf("handler %d\n", i);
	Sleep(10);
}
int _tmain(int argc, _TCHAR* argv[])
{
	printf("Main TID %d!\n", GetCurrentThreadId()); 

	task_service t;
	t.start_thread(1);

	t.post(std::bind(handler, 1) ,1);
	t.post(std::bind(handler, 2) ,2);
	t.post(std::bind(handler, 3),3);

	getchar();
	t.terminate_thread();
	return 0;
}

