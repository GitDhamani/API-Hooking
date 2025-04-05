#include <easyhook.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <iostream>

typedef BOOL(WINAPI* WriteFile_t)(
    HANDLE, const void*, DWORD, LPDWORD, LPVOID
);

WriteFile_t OriginalWriteFile = nullptr;

// Hooked function
BOOL WINAPI HookedWriteFile(
    HANDLE hConsoleOutput,
    const void* lpBuffer,
    DWORD nNumberOfCharsToWrite,
    LPDWORD lpNumberOfCharsWritten,
    LPVOID lpReserved)
{
    std::string originalText((char*)lpBuffer, nNumberOfCharsToWrite);

	// Check the incoming text for the Exchange Greeting Text
	size_t pos = originalText.find("Welcome to the Exchange Rate Converter.");
	if (pos != std::string::npos)
	
	{
		//Launch Authenticator 
		std::cout<<"The Program is now protected with an Authenticator.\n";
		std::cout<<"Please enter your authorised password: ";
		std::string pw;
		std::cin>>pw;
		if (pw != "password")
		{
			std::cout<<"Incorrect Password. Exiting...\n";
			system("pause");
			exit(0);  // 0 = Success, non-zero = Failure from cstdlib
		}
		
		std::cout<<"Correct. Granting Access.. ";
		Sleep(3000);
		system("cls");
		
	}
	
	// Call the original function
	HMODULE hKernel32 = GetModuleHandle("Kernel32.dll");
    OriginalWriteFile = (WriteFile_t)GetProcAddress(hKernel32, "WriteFile");
    return OriginalWriteFile(
        hConsoleOutput, lpBuffer, nNumberOfCharsToWrite,
        lpNumberOfCharsWritten, lpReserved);
}


// EasyHook will be looking for this export to support DLL injection. If not found then 
// DLL injection will fail.
extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
	// Perform hooking
	HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook

	// Install the hook
	NTSTATUS result = LhInstallHook(
		GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "WriteFile"),
		HookedWriteFile,
		NULL,
		&hHook);
	if (FAILED(result))
	{
		std::wstring s(RtlGetLastErrorString());
		std::wcout << "Failed to install hook: ";
		std::wcout << s;
	}
	else 
	{
		//std::cout << "Hook installed successfully.";
	}

	// If the threadId in the ACL is set to 0,
	// then internally EasyHook uses GetCurrentThreadId()
	ULONG ACLEntries[1] = { 0 };

	// Disable the hook for the provided threadIds, enable for all others
	LhSetExclusiveACL(ACLEntries, 1, &hHook);

	return;
}