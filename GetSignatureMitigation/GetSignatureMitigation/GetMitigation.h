#pragma once
#include <windows.h>

class GetMitigation
{
public:
	bool OpenPID(int PID);
	void PrintMitigation();
private:
	HANDLE GM_HProcess;
};
