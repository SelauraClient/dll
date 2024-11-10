#include "Selaura.h"

using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::ApplicationModel::Core;

namespace {
    alignas(Selaura) char SelauraBuffer[sizeof(Selaura)] = {};
}

void onTestEvent(Events::TestEvent& event) {
    if (!event.isCancelled()) {
        std::string message = "Test event received with value: " + std::to_string(event.getTest());
        MessageBoxA(NULL, message.c_str(), "Test Event", MB_OK);
    } else {
        MessageBoxA(NULL, "Test event was cancelled.", "Test Event", MB_OK);
    }
}

void Selaura::init(HINSTANCE hInst) {
    new (SelauraBuffer) Selaura;

    const winrt::Windows::ApplicationModel::Package package = winrt::Windows::ApplicationModel::Package::Current();
    auto [major, minor, build, revision] = package.Id().Version();
    std::wstring version = std::to_wstring(major) + L"." + std::to_wstring(minor) + L"." + std::to_wstring(build) + L"." + std::to_wstring(revision);

    {
        std::wstring build_str = std::to_wstring(build);
        if (build_str.length() > 2) {
            build_str = build_str.substr(0, build_str.length() - 2);
        }

        std::wstring title_version = std::to_wstring(major) + L"." + std::to_wstring(minor) + L"." + build_str;
        std::wstring title = L"Selaura Client v" + title_version;
        CoreApplication::MainView().CoreWindow().DispatcherQueue().TryEnqueue([title = std::move(title)]() {
            ApplicationView::GetForCurrentView().Title(title);
        });
    }


    Selaura::get().listen<Events::TestEvent>(onTestEvent);

    Events::TestEvent event(1);
    Selaura::get().push(event);
}

Selaura& Selaura::get() noexcept {
    return *std::launder(reinterpret_cast<Selaura*>(SelauraBuffer));
}

Selaura::~Selaura() {
    //clean up, pick up, put away
}
