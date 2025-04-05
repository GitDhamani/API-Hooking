#include <iostream>
#include <string>
#include <cstring>
#include <easyhook.h>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <cstdlib>
#include <thread>
#include <chrono>

DWORD FindProcessID(const std::wstring& processName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        std::wcerr << L"Failed to create process snapshot." << std::endl;
        return 0;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32))
    {
        do
        {
            if (processName == pe32.szExeFile)
            {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return 0;
}


int main(int argc, char* argv[])
{
	//Variable to hold the PID once found
	DWORD pid;
	
	//Loop waiting for Process to Appear
	while (true)
	{
		
		//Search for Converter.exe to appear
		const std::wstring targetProcess = L"Converter.exe";
		while (true)
		{
			pid = FindProcessID(targetProcess);
			if (pid != 0) break;
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}

		WCHAR* dllToInject = L"HookDLL.dll";
			
		// Inject dllToInject into the target process Id
		NTSTATUS nt = RhInjectLibrary(
			pid,   						// The process to inject into
			0,           				// ThreadId to wake up upon injection
			EASYHOOK_INJECT_DEFAULT,
			NULL, 						// 32-bit Provided
			dllToInject,		 		// 64-bit provided
			NULL, 						// data to send to injected DLL entry point
			0							// size of data to send
		);

		if (nt != 0)
		{
			printf("RhInjectLibrary failed with error code = %d\n", nt);
			PWCHAR err = RtlGetLastErrorString();
			std::wcout << err << "\n";
		}
		else 
		{
			std::wcout << L"Library injected ssuccessfully.\n";
		}

		//Search for Converter.exe to Disappear
		while (true)
		{
			pid = FindProcessID(targetProcess);
			if (pid == 0) break;
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
	}
	return 0;
}