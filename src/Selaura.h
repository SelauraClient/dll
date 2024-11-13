#pragma once
#include <new>
#include <Windows.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <typeindex>
#include <d3d11.h>
#include <mutex>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Applicationmodel.core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.System.h>

#include "hook/HookManager.h"
#include "sdk/sdk.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

class Selaura {
public:
    // gets
    static Selaura& get() noexcept;

    HINSTANCE getCurrentModule();
    SelauraSDK::GameVersion getGameVersion();
    template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    void SafeRelease(T& ptr) {
        if (ptr) {
            if constexpr (std::is_base_of_v<IUnknown, typename std::remove_pointer_t<T>>) {
                ptr->Release();
            } else {
                delete ptr;
            }

            ptr = nullptr;
        }
    };

    // funcs
    void init(HINSTANCE hInst);
    void setMinecraftTitle(std::string_view title);
    ~Selaura();
private:
    HINSTANCE module;

    HookManager hookManager;
    SelauraSDK::GameVersion gameVersion;
};
