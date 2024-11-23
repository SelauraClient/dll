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
        createHiddenWindow();
        init();
    }

    ~LogManager() {
        if (hwndHidden) {
            DestroyWindow(hwndHidden);
        }
    }

    void init() {
        // Set up the log folder
        char path[MAX_PATH];
        ExpandEnvironmentStringsA("%localappdata%\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Selaura\\logs", path, MAX_PATH);
        logFolder = path;

        // Create the log directory if it doesn't exist
        std::filesystem::create_directories(logFolder);

        showMessage("LogManager initialized.", "Log Info");
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
    HWND hwndHidden = nullptr;

    void createHiddenWindow() {
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = DefWindowProc; // Default window procedure
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = "HiddenLogWindowClass";

        RegisterClass(&wc);
        hwndHidden = CreateWindow(wc.lpszClassName, "Hidden Log Window",
                                   WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                   0, 0, nullptr, nullptr, wc.hInstance, nullptr);
        ShowWindow(hwndHidden, SW_HIDE); // Hide the window
    }

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
            showMessage("Failed to open log file.", "Log Error", MB_ICONERROR);
        }
    }

    void showMessage(const std::string& message, const std::string& title, UINT iconType = MB_OK) {
        MessageBoxA(hwndHidden, message.c_str(), title.c_str(), iconType);
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
