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
        logFile.open(logFilePath, std::ios::app);
        if (!logFile.is_open()) {
            std::terminate();
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

    template<typename... Args>
    void log(const std::string& level, const Args&... args) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) {
            logFile << formatMessage(level, args...) << std::endl;
        } else {
            std::terminate();
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

        return std::format("[{}] [{}] {}", timestamp, level, std::vformat("{}", std::make_format_args(args...)));
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