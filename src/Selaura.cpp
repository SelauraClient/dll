#include "Selaura.h"

namespace {
    alignas(Selaura) char SelauraBuffer[sizeof(Selaura)] = {};
};

Selaura& Selaura::get() {
    return *std::launder(reinterpret_cast<Selaura*>(SelauraBuffer));
};

void Selaura::unload() {
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibraryAndExitThread, this->module, 0, nullptr);
}

LogManager& Selaura::getLogManager() {
    return this->logManager;
}

Selaura::Selaura(HINSTANCE mModule) : module(mModule) {
    this->logManager.init();
}

void Selaura::init() {
    MessageBoxA(NULL, "Hello, World!", "Message Box Title", MB_OK);
    this->logManager.info("hello world!");
}