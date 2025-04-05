# API-Hooking
How to use the EasyHook Library to perform API Hooking
First build the target application using CL from Visual Studio's Dev Prompt. This will ensure it imports the WriteFile API from Kernel32.dll.
Build everything else using CMake using the MSVC Generator.

Run the Injector first and make sure the HookDLL.dll and EasyHook64.DLLs are both accessible in the same location, or in your system path.
Now when you run the Converter, it will have Authentication added via DLL Injection.

If you wish to explore and build the EasyHook Library yourself, you can find it here: 
https://github.com/EasyHook/EasyHook
