#include "AIAssistant.h"
#include <nlohmann/json.hpp>

AIAssistant::AIAssistant(const std::string& apiKey) : apiKey(apiKey) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

AIAssistant::~AIAssistant() {
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

void AIAssistant::getSuggestions(const std::string& codeContext, std::function<void(const std::string&)> callback) {
    std::thread([this, codeContext, callback]() {
        if (curl) {
            std::string readBuffer;
            std::string url = "https://api.openai.com/v1/completions";
            
            nlohmann::json payload = {
                {"model", "text-davinci-002"},
                {"prompt", "Complete this code:\n" + codeContext},
                {"max_tokens", 50},
                {"n", 1},
                {"stop", nullptr}
            };

            std::string payloadStr = payload.dump();

            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payloadStr.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            CURLcode res = curl_easy_perform(curl);
            
            if (res == CURLE_OK) {
                nlohmann::json response = nlohmann::json::parse(readBuffer);
                std::string suggestion = response["choices"][0]["text"];
                callback(suggestion);
            } else {
                callback("Error: " + std::string(curl_easy_strerror(res)));
            }

            curl_slist_free_all(headers);
        }
    }).detach();
}

size_t AIAssistant::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
        return newLength;
    } catch(std::bad_alloc &e) {
        return 0;
    }
}