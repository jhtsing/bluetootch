#ifndef __SIMMULATION_KEY_H__
#define __SIMMULATION_KEY_H__
#include <string>

const std::string cmd_next = "next";
const std::string cmd_pre = "pre";
const std::string cmd_exit = "esc";
const std::string cmd_start = "start";
const std::string cmd_stop = "stop";
class SimulationKey
{
public:
	SimulationKey();
	~SimulationKey();
	//从头开始播放
	void SetF5();
	//上一页
	void SetLeft();
	//下一页
	void SetRight();
	//推出放映
	void SetEsc();
	//从当前页开始播放
	void SetShiltF5();

	std::wstring SetWindowsWnd(const std::wstring& wnd_class);
private:
	HWND ppt_windows_handle_;
};
#endif
