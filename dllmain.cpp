#include <Windows.h>
#include <mutex>

#include "src/Selaura.h"

DWORD WINAPI StartRoutine(HINSTANCE hinstDLL) {
    MessageBoxA(NULL, "loaded", "ok", MB_OK | MB_ICONERROR);
    Selaura::get().init();
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lp) {
    if (GetModuleHandleA("Minecraft.Windows.exe") != GetModuleHandleA(NULL)) {
        return TRUE;
    }

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)StartRoutine, hinstDLL, 0, nullptr));
    }

    if (fdwReason == DLL_PROCESS_DETACH) {
        Selaura::get().unload();
    }

    return TRUE;
}
