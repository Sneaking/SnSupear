#include "ErrorHandler.h"

void ErrorHandler::showError(HWND hwnd, const std::string& message) {
    MessageBoxA(hwnd, message.c_str(), "Error", MB_OK | MB_ICONERROR);
}

void ErrorHandler::showWarning(HWND hwnd, const std::string& message) {
    MessageBoxA(hwnd, message.c_str(), "Warning", MB_OK | MB_ICONWARNING);
}

void ErrorHandler::showInfo(HWND hwnd, const std::string& message) {
    MessageBoxA(hwnd, message.c_str(), "Information", MB_OK | MB_ICONINFORMATION);
}