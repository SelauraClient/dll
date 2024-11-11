#include "Selaura.h"

using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::ApplicationModel::Core;

namespace {
    alignas(Selaura) char SelauraBuffer[sizeof(Selaura)] = {};
}

void Selaura::init(HINSTANCE hInst) {
    new (SelauraBuffer) Selaura;

    const winrt::Windows::ApplicationModel::Package package = winrt::Windows::ApplicationModel::Package::Current();
    auto [major, minor, build, revision] = package.Id().Version();
    std::wstring version = std::to_wstring(major) + L"." + std::to_wstring(minor) + L"." + std::to_wstring(build) + L"." + std::to_wstring(revision);

    { // window title
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

    this->module = hInst;
}

Selaura& Selaura::get() noexcept {
    return *std::launder(reinterpret_cast<Selaura*>(SelauraBuffer));
}

HINSTANCE Selaura::getCurrentModule() {
    return this->module;
}

template<typename T>
void Selaura::push(T&& event) {
    notify(std::type_index(typeid(T)), static_cast<void*>(&event));
}

template<typename T>
void Selaura::listen(std::function<void(T)> callback) {
    auto wrapper = [callback](void* data) {
        callback(*static_cast<T*>(data));
    };
    listeners[std::type_index(typeid(T))].emplace_back(wrapper);
}

void Selaura::notify(const std::type_index& eventType, void* eventData) {
    auto it = listeners.find(eventType);
    if (it != listeners.end()) {
        for (auto& listener : it->second) {
            listener(eventData);
        }
    }
}

Selaura::~Selaura() {
    //clean up, pick up, put away
}