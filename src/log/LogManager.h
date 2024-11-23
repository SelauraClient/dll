#pragma once

#include <filesystem>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <iostream>
#include <format>
#include <Windows.h>
#include <fstream>

class LogManager {
public:
    LogManager() : logFilePath("") {}

    ~LogManager() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void init() {
        char path[MAX_PATH];
        ExpandEnvironmentStringsA("%localappdata%\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Selaura\\logs", path, MAX_PATH);
        logFolder = path;

        std::filesystem::create_directories(logFolder);
        logFilePath = getLogFilePath();
        
        // Attempt to open the log file and handle errors
        logFile.open(logFilePath, std::ios::app);
        if (!logFile.is_open()) {
            showError("Failed to open log file.");
        }
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
    std::string logFilePath;
    std::ofstream logFile;
    std::mutex logMutex;

    void showError(const std::string& message) const {
        MessageBoxA(nullptr, message.c_str(), "Error", MB_ICONERROR | MB_OK);
    }

    template<typename... Args>
    void log(const std::string& level, const Args&... args) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (!logFile.is_open()) {
            showError("Log file is not open.");
            return; // Prevent further execution if log file is not available
        }
        
        try {
            logFile << formatMessage(level, args...) << std::endl;
        } catch (const std::exception& e) {
            showError(std::string("Failed to write log: ") + e.what());
        }
    }

    template<typename... Args>
    std::string formatMessage(const std::string& level, const Args&... args) const {
        auto timeNow = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(timeNow);
        std::tm buf;
        localtime_s(&buf, &timeT);

        std::ostringstream timestampStream;
        timestampStream << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
        std::string timestamp = timestampStream.str();

        // Format the message safely
        try {
            return std::format("[{}] [{}] {}", timestamp, level, std::vformat("{}", std::make_format_args(args...)));
        } catch (const std::exception& e) {
            showError(std::string("Formatting error: ") + e.what());
            return "[ERROR] [LOG MESSAGE FORMAT ERROR]"; // Fallback message
        }
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
