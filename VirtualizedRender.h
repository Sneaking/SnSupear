#pragma once
#include <string>
#include <vector>
#include <windows.h>

class VirtualizedRender {
public:
    VirtualizedRender(HWND hEdit);
    void updateVisibleLines();
    void renderVisibleLines();

private:
    HWND hEdit;
    std::vector<std::string> lines;
    int firstVisibleLine;
    int visibleLineCount;
};