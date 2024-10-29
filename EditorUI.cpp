#include "SyntaxHighlighter.h"
#include "AIAssistant.h"
#include "VirtualizedRender.h"
#include "ErrorHandler.h"
#include <string>
#include <vector>
#include <algorithm>

class EditorUI {
private:
    HWND hEdit;
    SyntaxHighlighter syntaxHighlighter;
    AIAssistant aiAssistant;
    VirtualizedRender virtualizedRender;
    std::vector<CHARRANGE> selections;

    void handleMultipleSelections(WPARAM wParam, LPARAM lParam);
    void showGotoAnything();

public:
    EditorUI(HWND hEdit, const std::string& apiKey)
        : hEdit(hEdit), aiAssistant(apiKey), virtualizedRender(hEdit) {}

    LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

LRESULT CALLBACK EditorUI::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Initialize RichEdit control
            LoadLibrary(TEXT("Msftedit.dll"));
            hEdit = CreateWindowEx(
                0, L"RICHEDIT50W", L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
                0, 0, width, height,
                hwnd, (HMENU)IDC_MAIN_EDIT, hInstance, NULL
            );
            
            // Set Marina theme colors
            {
                CHARFORMAT2 cf = {0};
                cf.cbSize = sizeof(CHARFORMAT2);
                cf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
                cf.crTextColor = RGB(144, 0, 0);  // Dark red for default text
                cf.crBackColor = RGB(1, 0, 0);    // Very dark green background
                SendMessage(hEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);

                // Set caret color
                SendMessage(hEdit, EM_SETCARETCOLOR, FALSE, RGB(0, 255, 0));  // Green caret
            }
            break;

        case WM_COMMAND:
            if (HIWORD(wParam) == EN_CHANGE && (HWND)lParam == hEdit) {
                int textLength = GetWindowTextLength(hEdit);
                std::wstring text(textLength + 1, L'\0');
                GetWindowText(hEdit, &text[0], textLength + 1);
                syntaxHighlighter.applyHighlighting(hEdit, text);
            }
            break;

        case WM_KEYDOWN:
            if (wParam == VK_CONTROL) {
                // Start multiple selection mode
                handleMultipleSelections(wParam, lParam);
            } else if (wParam == 'P' && GetKeyState(VK_CONTROL) < 0) {
                // Ctrl+P for Goto Anything
                showGotoAnything();
            }
            break;

        // ... (rest of the WndProc function)
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void EditorUI::handleMultipleSelections(WPARAM wParam, LPARAM lParam) {
    if (wParam == VK_CONTROL) {
        CHARRANGE cr;
        SendMessage(hEdit, EM_EXGETSEL, 0, (LPARAM)&cr);
        
        // Add new selection if it doesn't overlap with existing ones
        if (std::none_of(selections.begin(), selections.end(), 
            [&cr](const CHARRANGE& existing) {
                return (cr.cpMin >= existing.cpMin && cr.cpMin <= existing.cpMax) ||
                       (cr.cpMax >= existing.cpMin && cr.cpMax <= existing.cpMax);
            })) {
            selections.push_back(cr);
        }
    }
}

void EditorUI::showGotoAnything() {
    // Create a simple dialog for Goto Anything
    HWND hDlg = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_GOTO_ANYTHING), hEdit, GotoAnythingDialogProc);
    ShowWindow(hDlg, SW_SHOW);
}

// Goto Anything dialog procedure
INT_PTR CALLBACK GotoAnythingDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                TCHAR szBuffer[256];
                GetDlgItemText(hDlg, IDC_GOTO_EDIT, szBuffer, 256);
                // Implement the logic to go to the entered location
                EndDialog(hDlg, IDOK);
                return TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, IDCANCEL);
                return TRUE;
            }
            break;
    }
    return FALSE;
}