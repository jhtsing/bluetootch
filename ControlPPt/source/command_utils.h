#ifndef __COMMAND_UTILS_H__
#define __COMMAND_UTILS_H__
#include <Windows.h>

namespace CommandUtils
{
	void SetF5()
	{
		keybd_event(VK_F5,0,0,0);
		keybd_event(VK_F5,0,KEYEVENTF_KEYUP,0);
	}
	void SetLeft()
	{
		keybd_event(VK_LEFT,0,0,0);
		keybd_event(VK_LEFT,0,KEYEVENTF_KEYUP,0);
	}
	void SetRight()
	{
		keybd_event(VK_RIGHT,0,0,0);
		keybd_event(VK_RIGHT,0,KEYEVENTF_KEYUP,0);
	}
	void SetEsc()
	{	
		keybd_event(VK_ESCAPE,0,0,0);
		keybd_event(VK_ESCAPE,0,KEYEVENTF_KEYUP,0);
	}
	void SetShiltF5()
	{
		keybd_event(VK_SHIFT,0,0,0);                                     //模拟先按下Shift键
		keybd_event(VK_F5,0,0,0);                                               // 在没有弹出来的情况下按左键盘的数字“2”按键
		keybd_event(VK_F5,0,KEYEVENTF_KEYUP,0);          //然后松开键盘
		keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);//然后松开键盘
	}
}
#endif // !__COMMAND_UTILS_H__
