#include <iostream>
#include "text_buffer.h"
#include "syntax_highlighter.h"
#include "code_completion.h"
#include "EditorUI.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize COM for the AI assistant
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    // Create the main window
    EditorUI editor(hInstance);
    if (!editor.Create()) {
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

    return (int)msg.wParam;
}