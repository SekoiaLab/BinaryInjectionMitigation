#include "stdafx.h"
#include <iostream>
#include "GetMitigation.h"
#include "SetPriv.h"
using namespace std;

void usage(char *binary)
{
	cerr << "Usage: " << binary << " PID" << endl;
	exit(1);
}


int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		//Get SeDebugPrivilege
		SetPriv debug;
		cout << "Get SeDebugPrivilege: ";
		if (!debug.CheckPriv())
		{
			cout << "Cannot obtain the adjust|query privilege." << endl;
			exit(1);
		}
		if (!debug.Set(SE_DEBUG_NAME, TRUE))
		{
			cout << "Cannot obtain the Debug privilege." << endl;
			exit(1);
		}
		cout << "OK" << endl;

		//GetMitigation
		GetMitigation hGM;
		int PID = atoi(argv[1]);
		cout << "Open process " << PID << ": ";
		if (hGM.OpenPID(PID))
		{
			cout << "OK" << endl;
			hGM.PrintMitigation();
		}
		else {
			cout << "KO" << endl;
		}
	}
	else {
		usage(argv[0]);
	}
}

