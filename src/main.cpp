#include <iostream>
#include "text_buffer.h"
#include "syntax_highlighter.h"
#include "code_completion.h"
#include "EditorUI.h"

/**
 * @brief Main entry point for the Sn_Supear application.
 * 
 * Initializes COM, creates the main window, and runs the message loop.
 * 
 * @param hInstance Instance handle for the application.
 * @param hPrevInstance Handle to the previous instance of the application (unused).
 * @param lpCmdLine Command line arguments (unused).
 * @param nCmdShow Window show command (unused).
 * @return Exit code of the application.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize COM for the AI assistant
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM." << std::endl;
        return 1;
    }

    // Create the main window
    EditorUI editor(hInstance);
    if (!editor.Create()) {
        std::cerr << "Failed to create main window." << std::endl;
        CoUninitialize();
        return 1;
    }

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Uninitialize COM
    CoUninitialize();

    return static_cast<int>(msg.wParam);
}
