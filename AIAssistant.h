#pragma once
#include <string>
#include <functional>
#include <thread>
#include <curl/curl.h>

class AIAssistant {
public:
    AIAssistant(const std::string& apiKey);
    ~AIAssistant();
    void getSuggestions(const std::string& codeContext, std::function<void(const std::string&)> callback);

private:
    std::string apiKey;
    CURL* curl;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};