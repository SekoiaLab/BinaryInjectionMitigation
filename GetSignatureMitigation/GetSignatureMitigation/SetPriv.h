#pragma once
#include <Windows.h>
class SetPriv
{
public:
	bool CheckPriv();
	bool Set(LPCTSTR Privilege, BOOL bEnablePrivilege);
private:
	HANDLE Token;
};

