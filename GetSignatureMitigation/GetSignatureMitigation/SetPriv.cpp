#include "stdafx.h"
#include "SetPriv.h"
#include <Windows.h>
#include <iostream>
using namespace std;


bool SetPriv::CheckPriv()
{
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		Token = hToken;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

bool SetPriv::Set(LPCTSTR Privilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp = { 0 };
	LUID luid;
	DWORD cb = sizeof(TOKEN_PRIVILEGES);
	if (!LookupPrivilegeValue(NULL, Privilege, &luid))
		return FALSE;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege) {
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else {
		tp.Privileges[0].Attributes = 0;
	}
	AdjustTokenPrivileges(Token, FALSE, &tp, cb, NULL, NULL);
	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
