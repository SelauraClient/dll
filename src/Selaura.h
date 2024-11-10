#pragma once
#include <new>
#include <Windows.h>
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <iostream>

#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.System.h>

#include "event/Events.h"

class Selaura {
public:
    static Selaura& get() noexcept;

    void init(HINSTANCE hInst);

    /*template<typename EventType>
    using EventHandler = std::function<void(EventType&)>;

    template<typename EventType>
    void listen(EventHandler<EventType> handler) {
        auto& handlers = eventHandlers[typeid(EventType)];
        handlers.emplace_back([handler](void* eventPtr) {
            handler(*static_cast<EventType*>(eventPtr));
        });
    }

    template<typename EventType>
    void push(EventType& event) {
        auto it = eventHandlers.find(typeid(EventType));
        if (it != eventHandlers.end()) {
            for (const auto& handler : it->second) {
                handler(static_cast<void*>(&event));
            }
        }
    }*/

    ~Selaura();

private:
    std::unordered_map<std::type_index, std::vector<std::function<void(void*)>>> eventHandlers;
};
