#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lp) {
    if (GetModuleHandleA("Minecraft.Windows.exe") != GetModuleHandleA(NULL)) {
    	// app injected is NOT Minecraft
    	return TRUE;
    }

    if (fdwReason == DLL_PROCESS_ATTACH) {
    	// soon
    }
	
    return TRUE;
}