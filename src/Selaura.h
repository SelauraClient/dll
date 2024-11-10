#pragma once
#include <new>
#include <Windows.h> // Add this line to include HINSTANCE

class Selaura {
public:
    // gets
    static Selaura& get() noexcept;

    // funcs
    void init(HINSTANCE hInst);
};
