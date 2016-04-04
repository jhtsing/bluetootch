// PPTServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <memory>
#include <Windows.h>
#include "control_center.h"
int main()
{
	std::shared_ptr<ControlCenter> control_center_(new ControlCenter());
	if (!control_center_) {
		printf("crate control_center failed!\n");
		return 0;
	}
	if (!control_center_->Init()) {
		printf("control_center init failed!\n");
		return 0;
	}
	if (!control_center_->Start()) {
		printf("control_center start failed!\n");
		return 0;
	}
	control_center_->Stop();
	control_center_->UnInit();
	getchar();
	return 1;
}

