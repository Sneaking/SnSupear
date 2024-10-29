#include "VirtualizedRender.h"

VirtualizedRender::VirtualizedRender(HWND hEdit) : hEdit(hEdit), firstVisibleLine(0), visibleLineCount(0) {}

void VirtualizedRender::updateVisibleLines() {
    RECT rect;
    GetClientRect(hEdit, &rect);
    int lineHeight = SendMessage(hEdit, EM_GETLINEHEIGHT, 0, 0);
    
    firstVisibleLine = SendMessage(hEdit, EM_GETFIRSTVISIBLELINE, 0, 0);
    visibleLineCount = (rect.bottom - rect.top) / lineHeight + 1;
}

void VirtualizedRender::renderVisibleLines() {
    // Clear the current content
    SetWindowText(hEdit, "");

    // Render only the visible lines
    for (int i = firstVisibleLine; i < firstVisibleLine + visibleLineCount && i < lines.size(); ++i) {
        SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)lines[i].c_str());
    }
}