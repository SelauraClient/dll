#include "Selaura.h"

namespace {
    // buffers for saving
    alignas(Selaura) char SelauraBuffer[sizeof(Selaura)] = {};
}

void Selaura::init(HINSTANCE hInst) {
    new (SelauraBuffer) Selaura;
}

Selaura& Selaura::get() noexcept {
    return *std::launder(reinterpret_cast<Selaura*>(SelauraBuffer));
}
