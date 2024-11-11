#pragma once
#include <new>
#include <Windows.h>

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

    // event system
    template<typename T>
    void push(T&& event);

    template<typename T>
    void listen(std::function<void(T)> callback);

    void notify(const std::type_index& eventType, void* eventData);

private:
    HINSTANCE module;
};