# BinaryInjectionMitigation
Two tools used during our analysis of the Microsoft binary injection mitigation implemented in Edge TH2.

## Tool 1: GetSignatureMitigation
The purpose of this tool is to list the signature mitigation flags in a running process.
To compile this binary, please download and install the latest Windows SDK version (in my case Version 1511 - updated on November 30th, 2015).

Example of usage on the PID 3704 (MicrosoftEdgeCP.exe in my case):

```
C:\>GetSignatureMitigation.exe 3704
Get SeDebugPrivilege: OK
Open process 3704: OK

Signature Microsoft Signed Only: 0
Signature Store Signed Only: 1
Signature Mitigation Opt-In: 1
```

The code used the following undocumented structure:

```
typedef enum _PROCESS_MITIGATION_POLICY {
    ProcessDEPPolicy,
    ProcessASLRPolicy,
    ProcessDynamicCodePolicy,
    ProcessStrictHandleCheckPolicy,
    ProcessSystemCallDisablePolicy,
    ProcessMitigationOptionsMask,
    ProcessExtensionPointDisablePolicy,
    ProcessControlFlowGuardPolicy,
    ProcessSignaturePolicy,
    ProcessFontDisablePolicy,
    ProcessImageLoadPolicy,
    MaxProcessMitigationPolicy
} PROCESS_MITIGATION_POLICY, *PPROCESS_MITIGATION_POLICY;

typedef struct _PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY {
    union {
        DWORD Flags;
        struct {
            DWORD MicrosoftSignedOnly : 1;
            DWORD StoreSignedOnly : 1;
            DWORD MitigationOptIn : 1;
            DWORD ReservedFlags : 29;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;
} PROCESS_MITIGATION_BINARY_SIGNATURE_POLICY, *PPROCESS_MITIGATION_BINARY_SIGNATURE_POLICY;
```

## Tool 2: injector
The purpose of this tool is to inject code in a running process. We created it to test the signature loading mitigation of Edge TH2.
Here is the syntax:

```
C:\>injector.exe
Usage of the injector.

injector.exe /d dll_file PID
injector.exe /s shellcode_file PID
    /d dll_file PID: dll injection via LoadLibrary().
    /s shellcode_file PID: shellcode injection.
```

The /d option uses the LoadLibrary() function in order to load a DLL.
the /s option push the shellcode in memory and execute it directly.

P.
