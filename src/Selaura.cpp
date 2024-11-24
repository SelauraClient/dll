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
    Selaura::get().logManager.info("hiii");
}