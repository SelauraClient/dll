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

class LogManager {
public:
    LogManager() {
        init();
    }

    void init() {
        // Set up the log folder
        char path[MAX_PATH];
        ExpandEnvironmentStringsA("%localappdata%\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Selaura\\logs", path, MAX_PATH);
        logFolder = path;

        // Create the log directory if it doesn't exist
        std::filesystem::create_directories(logFolder);

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
    std::string logFolder;
    std::mutex logMutex;

    template<typename... Args>
    void log(const std::string& level, const Args&... args) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::string message = formatMessage(level, args...);

        // Write the log message to a file
        std::ofstream logFile(getLogFilePath(), std::ios::app);
        if (logFile.is_open()) {
            logFile << message << std::endl;
            logFile.close();
        } else {
            MessageBoxA(nullptr, "Failed to open log file.", "Log Error", MB_OK | MB_ICONERROR);
        }
    }

    template<typename... Args>
    std::string formatMessage(const std::string& level, const Args&... args) const {
        std::ostringstream oss;
        oss << "[" << level << "] " << std::vformat("{}", std::make_format_args(args...));
        return oss.str();
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
