#pragma once
#include <string>
#include <windows.h>

class ErrorHandler {
public:
    static void showError(HWND hwnd, const std::string& message);
    static void showWarning(HWND hwnd, const std::string& message);
    static void showInfo(HWND hwnd, const std::string& message);
};