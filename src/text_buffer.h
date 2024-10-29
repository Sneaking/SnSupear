#pragma once
#include <string>
#include <vector>

class TextBuffer {
public:
    void loadFile(const std::string& filename);
    void saveFile(const std::string& filename);
    void insertText(const std::string& text, size_t position);
    void deleteText(size_t start, size_t end);
    std::string getBuffer() const;
    std::string getLine(size_t lineNumber) const;
    size_t getLineCount() const;

private:
    std::vector<std::string> lines;
};