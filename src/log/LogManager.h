#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <Windows.h>

class LogManager {
public:
    LogManager() = default;

    void init() {
        MessageBoxA(nullptr, "LogManager initialized.", "Log Info", MB_OK);
    }

    template<typename... Args>
    void info(const Args&... args) {
        log("INFO", args...);
    }

    template<typename... Args>
    void warn(const Args&... args) {
        log("WARN", args...);
    }

    template<typename... Args>
    void error(const Args&... args) {
        log("ERROR", args...);
    }

private:
    std::mutex logMutex;

    template<typename... Args>
    void log(const std::string& level, const Args&... args) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::string message = formatMessage(level, args...);
        MessageBoxA(nullptr, message.c_str(), "Log Message", MB_OK);
    }

    template<typename... Args>
    std::string formatMessage(const std::string& level, const Args&... args) const {
        std::ostringstream oss;
        oss << "[" << level << "] " << std::vformat("{}", std::make_format_args(args...));
        return oss.str();
    }
};
