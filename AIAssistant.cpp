// AIAssistant.cpp
#include "AIAssistant.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QPlainTextEdit>

/**
 * @brief Constructs the AIAssistant widget.
 * @param parent The parent widget.
 */
AIAssistant::AIAssistant(QWidget* parent)
    : QWidget(parent)
    , networkManager(new QNetworkAccessManager(this))
    , responseDisplay(new QPlainTextEdit(this))
{
    setupUI();

    // Connect the network manager to handle responses
    connect(networkManager, &QNetworkAccessManager::finished, this, &AIAssistant::onNetworkReply);
}

/**
 * @brief Sets up the UI elements of the widget.
 */
void AIAssistant::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    responseDisplay->setReadOnly(true);
    layout->addWidget(responseDisplay);
    setLayout(layout);
}

/**
 * @brief Retrieves the API key for the AI service.
 * @return The API key as a QString.
 */
QString AIAssistant::getAPIKey() const {
    // Replace with secure key retrieval in production
    return qgetenv("OPENAI_API_KEY");
}

/**
 * @brief Sends a prompt to the AI assistant.
 * @param prompt The prompt text to send.
 */
void AIAssistant::sendPrompt(const QString& prompt) {
    QUrl apiUrl("https://api.openai.com/v1/chat/completions");
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + getAPIKey().toUtf8());

    // Create the JSON payload for the request
    QJsonObject payload;
    QJsonArray messages;
    QJsonObject message;
    message["role"] = "user";
    message["content"] = prompt;
    messages.append(message);
    payload["model"] = "gpt-3.5-turbo";
    payload["messages"] = messages;
    payload["max_tokens"] = 150;

    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    // Send the request
    networkManager->post(request, data);
}

/**
 * @brief Handles the response from the AI API.
 * @param reply The network reply containing the response.
 */
void AIAssistant::onNetworkReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Error", "Network request failed: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    // Parse the JSON response
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    QJsonObject responseObject = jsonResponse.object();

    // Extract and display the AI's response
    if (responseObject.contains("choices") && responseObject["choices"].isArray()) {
        QJsonArray choices = responseObject["choices"].toArray();
        if (!choices.isEmpty() && choices[0].isObject()) {
            QJsonObject choice = choices[0].toObject();
            if (choice.contains("message") && choice["message"].isObject()) {
                QJsonObject message = choice["message"].toObject();
                if (message.contains("content") && message["content"].isString()) {
                    QString aiResponse = message["content"].toString();
                    responseDisplay->appendPlainText(aiResponse);
                    emit responseReceived(aiResponse);
                }
            }
        }
    } else {
        QMessageBox::warning(this, "Response Error", "Unexpected response format");
    }

    reply->deleteLater();
}
