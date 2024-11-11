#pragma once
#include <new>
#include <Windows.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>

#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Applicationmodel.core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.System.h>

class Selaura {
public:
    // gets
    static Selaura& get() noexcept;

    HINSTANCE getCurrentModule();

    // funcs
    void init(HINSTANCE hInst);
    ~Selaura();
private:
    HINSTANCE module;

    HookManager hookManager;
};