#include "SyntaxHighlighter.h"
#include <regex>

void SyntaxHighlighter::applyHighlighting(HWND hEdit, const std::wstring& text) {
    // Simple regex for C++ keywords (expand as needed)
    std::wregex keywordRegex(L"\\b(int|float|double|char|void|for|while|if|else|return)\\b");

    std::wsregex_iterator it(text.begin(), text.end(), keywordRegex);
    std::wsregex_iterator end;

    while (it != end) {
        CHARRANGE cr;
        CHARFORMAT2 cf = {0};

        cr.cpMin = it->position();
        cr.cpMax = cr.cpMin + it->length();

        cf.cbSize = sizeof(CHARFORMAT2);
        cf.dwMask = CFM_COLOR;
        cf.crTextColor = RGB(0, 0, 255); // Blue color for keywords

        SendMessage(hEdit, EM_EXSETSEL, 0, (LPARAM)&cr);
        SendMessage(hEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);

        ++it;
    }

    // Reset selection
    CHARRANGE resetCr = {-1, -1};
    SendMessage(hEdit, EM_EXSETSEL, 0, (LPARAM)&resetCr);
}