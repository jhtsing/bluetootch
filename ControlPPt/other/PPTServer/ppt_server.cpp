// PPTServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>


int main()
{
	HWND hw = FindWindow(_T("PPTFrameClass"), NULL);
	if (hw == NULL){
		printf("failed");
	}
	TCHAR lpWinTitle[256] = {0};
	::GetWindowText(hw, lpWinTitle, 256 - 1);
    return 0;
}

