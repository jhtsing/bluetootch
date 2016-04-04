#include "stdafx.h"
#include <Windows.h>
#include "simulation_key.h"

SimulationKey::SimulationKey():
ppt_windows_handle_(nullptr){

}
SimulationKey::~SimulationKey(){

}
void SimulationKey::SetF5(){
	keybd_event(VK_F5, 0, 0, 0);
	keybd_event(VK_F5, 0, KEYEVENTF_KEYUP, 0);
}
void SimulationKey::SetLeft(){
		
	keybd_event(VK_LEFT, 0, 0, 0);
	keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);
}
	void SimulationKey::SetRight(){

	keybd_event(VK_RIGHT, 0, 0, 0);
	keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
}
void SimulationKey::SetEsc(){
	keybd_event(VK_ESCAPE, 0, 0, 0);
	keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
}
void SimulationKey::SetShiltF5(){

	keybd_event(VK_SHIFT, 0, 0, 0);                                     //模拟先按下Shift键
	keybd_event(VK_F5, 0, 0, 0);                                               // 在没有弹出来的情况下按左键盘的数字“2”按键
	keybd_event(VK_F5, 0, KEYEVENTF_KEYUP, 0);          //然后松开键盘
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);//然后松开键盘
}
bool SetPPTPageFrontShow(HWND hw) {
	if (GetForegroundWindow() == hw) {
		return true;
	}
	typedef void (WINAPI *PROCSWITCHTOTHISWINDOW)(HWND, BOOL);
	HMODULE hUser32 = GetModuleHandle(L"user32.dll");
	if (nullptr == hUser32) {
		return false;
	}
	PROCSWITCHTOTHISWINDOW SwitchToThisWindow = (PROCSWITCHTOTHISWINDOW)GetProcAddress(hUser32, "SwitchToThisWindow");
	if (nullptr == SwitchToThisWindow) {
		return false;
	}
	SwitchToThisWindow(hw, TRUE);
	return TRUE;
}
std::wstring SimulationKey::SetWindowsWnd(const std::wstring& wnd_class) {
	HWND hw = FindWindow(wnd_class.c_str(), NULL);
	if (hw == NULL) {
		printf("failed");
		return L"failed";
	}
	TCHAR lpWinTitle[256] = { 0 };
	::GetWindowText(hw, lpWinTitle, 256 - 1);

	return lpWinTitle;
}
