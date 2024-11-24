#pragma once

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <mutex>
#include <Windows.h>
#include <filesystem>
#include <cstdio>
#include <format>
#include <unordered_map>

class LogManager {
public:
    void init() {
        std::filesystem::path logDir = logFilePath.parent_path();
        if (!std::filesystem::exists(logDir)) {
            std::filesystem::create_directories(logDir);
        }

        logFile = fopen(logFilePath.c_str(), "a+");
        if (!logFile) {
            MessageBoxA(NULL, "Failed to open log file!", "Error", MB_OK | MB_ICONERROR);
        }
    }
    
    ~LogManager() {
        if (logFile) {
            fclose(logFile);
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
        std::terminate();
    }

    void resetLog() {
        if (logFile) {
            fclose(logFile);
        }
        logFile = fopen(logFilePath.c_str(), "w+");
    }

    void createDir(const std::string& path) {
        std::filesystem::path dirPath = logFilePath.parent_path() / path;
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directories(dirPath);
        }
    }

    void deletePath(const std::string& path) {
        std::filesystem::path precisePath = logFilePath.parent_path() / path;
        if (std::filesystem::exists(precisePath)) {
            std::filesystem::remove_all(precisePath);
        }
    }

    std::string readFile(const std::string& path) {
        std::ifstream fileInput(path);
        if (!fileInput) return "";

        std::ostringstream content;
        content << fileInput.rdbuf();
        return content.str();
    }

private:
    FILE* logFile = nullptr;
    const std::string logFilePath = std::string(getenv("localappdata")) + "\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Selaura\\logs.txt";
    std::mutex logMutex;

    template<typename... Args>
    std::string format(const Args&... args) {
        return std::vformat("{}", std::make_format_args(args...));
    }

    void log(const std::string& level, const std::string& message) {
        std::lock_guard<std::mutex> guard(logMutex);
        if (logFile) {
            fprintf(logFile, "[%s] [%s] %s\n", level.c_str(), getCurrentTime().c_str(), message.c_str());
            fflush(logFile);
        }
    }

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&now_time);

        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%m/%d/%Y %H:%M:%S");
        return oss.str();
    }

    void writeToFile(const std::string& content, const std::string& path) {
        std::ofstream fileOutput(path, std::ios::trunc);
        if (fileOutput) {
            fileOutput << content;
        }
    }
};