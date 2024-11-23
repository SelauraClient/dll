#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <iostream>
#include <format>
#include <Windows.h>

class LogManager {
public:
    LogManager() = default;

    void init() {
        char path[MAX_PATH];
        ExpandEnvironmentStringsA("%localappdata%\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Selaura\\logs", path, MAX_PATH);
        logFolder = path;

        std::filesystem::create_directories(logFolder);
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
    std::string logFolder;
    std::mutex logMutex;

    template<typename... Args>
    void log(const std::string& level, const Args&... args) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::ofstream logFile(getLogFilePath(), std::ios::app);
        if (logFile.is_open()) {
            logFile << "[" << level << "] " << formatMessage(args...) << std::endl;
        } else {
            std::terminate();
        }
    }

    template<typename... Args>
    std::string formatMessage(const Args&... args) const {
        return std::vformat("{}", std::make_format_args(args...));
    }

    std::string getLogFilePath() const {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf;
        localtime_s(&buf, &in_time_t);
        std::ostringstream oss;
        oss << logFolder << "\\Selaura_"
            << std::put_time(&buf, "%Y-%m-%d_%H-%M-%S") << ".txt";
        return oss.str();
    }
};