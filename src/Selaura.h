#pragma once
#include <Windows.h>
#include <new>
#include <filesystem>
#include <string>

#include "log/LogManager.h"

class Selaura {
public:
    explicit Selaura(HINSTANCE module);

    static Selaura& get();
    void init();
    void unload();

    LogManager& getLogManager();
private:
    HINSTANCE module;

    LogManager logManager;
};