#pragma once
class SimulationKey
{
public:
	SimulationKey();
	~SimulationKey();
	void SetF5();
	void SetLeft();
	void SetRight();
	void SetEsc();
	void SetShiltF5();
private:
	HWND ppt_windows_handle_;
};

