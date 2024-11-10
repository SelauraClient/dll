#include <Windows.h>
#include "src/Selaura.h"

DWORD WINAPI StartRoutine(HINSTANCE hinstDLL) {
    if (GetModuleHandleA("Minecraft.Windows.exe") == NULL) {
        return 0;
    }

    Selaura::get().init(hinstDLL);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lp) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartRoutine, hinstDLL, 0, NULL);
        if (hThread) {
            CloseHandle(hThread);
        }
        Selaura::get().~Selaura();
    }

    return TRUE;
}
