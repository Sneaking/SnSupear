// AIAssistant.h
#pragma once

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPlainTextEdit>

/**
 * @brief Widget for interacting with an AI assistant (e.g., ChatGPT).
 */
class AIAssistant : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructs the AIAssistant widget.
     * @param parent The parent widget.
     */
    explicit AIAssistant(QWidget* parent = nullptr);

    /**
     * @brief Sends a prompt to the AI assistant.
     * @param prompt The prompt text to send.
     */
    void sendPrompt(const QString& prompt);

signals:
    /**
     * @brief Emitted when a response is received from the AI.
     * @param response The AI response text.
     */
    void responseReceived(const QString& response);

private slots:
    /**
     * @brief Handles the response from the AI API.
     * @param reply The network reply containing the response.
     */
    void onNetworkReply(QNetworkReply* reply);

private:
    /**
     * @brief Sets up the UI elements of the widget.
     */
    void setupUI();

    /**
     * @brief Retrieves the API key for the AI service.
     * @return The API key as a QString.
     */
    QString getAPIKey() const;

    QNetworkAccessManager* networkManager;  ///< Network manager for API requests.
    QPlainTextEdit* responseDisplay;      ///< Text edit to display AI responses.
};

#endif // AIASSISTANT_H
