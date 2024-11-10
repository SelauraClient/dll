#include <Windows.h>
#include "src/Selaura.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lp) {
    if (GetModuleHandleA("Minecraft.Windows.exe") != GetModuleHandleA(NULL)) {
    	// app injected is NOT Minecraft
    	return TRUE;
    }

    if (fdwReason == DLL_PROCESS_ATTACH) {
    	Selaura::get().init(hInstDLL);
    }
	
    return TRUE;
}
