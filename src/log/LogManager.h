#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <format>

class LogManager {
public:
    LogManager() {
        init();
    }

    void init() {
        std::filesystem::create_directories(logDirectory);
        logFilePath = logDirectory + "\\Selaura_" + getCurrentTimestamp() + ".txt";
        logFile.open(logFilePath, std::ios::app);
    }

    ~LogManager() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    template<typename... Args>
    void info(const Args&... args) {
        log("INFO", format(args...));
    }

    template<typename... Args>
    void warn(const Args&... args) {
        log("WARN", format(args...));
    }

    template<typename... Args>
    void error(const Args&... args) {
        log("ERROR", format(args...));
    }

private:
    std::ofstream logFile;
    std::string logFilePath;
    const std::string logDirectory = std::string(getenv("localappdata")) + "\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Selaura\\logs";
    std::mutex logMutex;

    std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&now_time);

        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%Y%m%d_%H%M%S");
        return oss.str();
    }

    template<typename... Args>
    std::string format(const Args&... args) {
        return std::format(args...);
    }

    void log(const std::string& level, const std::string& message) {
        std::lock_guard<std::mutex> guard(logMutex);
        if (logFile.is_open()) {
            logFile << "[" << level << "] [" << getCurrentTime() << "] " << message << std::endl;
        }
    }

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&now_time);

        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%H:%M:%S");
        return oss.str();
    }
};
