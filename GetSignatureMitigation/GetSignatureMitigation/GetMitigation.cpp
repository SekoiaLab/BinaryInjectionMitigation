#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "GetMitigation.h"
using namespace std;

bool GetMitigation::OpenPID(int PID)
{
	HANDLE HProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (HProcess != NULL)
	{
		GM_HProcess = HProcess;
		return TRUE;
	} else {
		return FALSE;
	}
}

void GetMitigation::PrintMitigation()
{
	cout << endl;
	try 
	{
		PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY sign_policy;
		if (GetProcessMitigationPolicy(GM_HProcess, ProcessSignaturePolicy, &sign_policy, sizeof(sign_policy)))
		{
			cout << "Signature Microsoft Signed Only: " << sign_policy.MicrosoftSignedOnly << endl;
			cout << "Signature Store Signed Only: " << sign_policy.StoreSignedOnly << endl;
			cout << "Signature Mitigation Opt-In: " << sign_policy.MitigationOptIn << endl;
		} else {
			cout << "Signature Microsoft Signed Only: KO" << endl;
		}
	} catch(int e) {
		cout << "ProcessSignaturePolicy error: ";
		cout << e << endl;
	}
	cout << endl;
}

