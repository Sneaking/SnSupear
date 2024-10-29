#pragma once
#include <string>
#include <windows.h>
#include <vector>
#include <regex>

struct SyntaxRule {
    std::wregex pattern;
    COLORREF color;
};

class SyntaxHighlighter {
public:
    SyntaxHighlighter();
    void applyHighlighting(HWND hEdit, const std::wstring& text);
private:
    std::vector<SyntaxRule> rules;
    void initializeRules();
};