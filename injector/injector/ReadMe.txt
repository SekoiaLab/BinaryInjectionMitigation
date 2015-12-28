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
