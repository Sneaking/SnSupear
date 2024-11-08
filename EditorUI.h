// EditorUI.h
#ifndef EDITOR_UI_H
#define EDITOR_UI_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QCompleter>
#include <QShortcut>
#include <QTimer>

#include "AIAssistant.h" 
#include "SyntaxHighlighter.h"
#include "CodeFormatter.h" 

class EditorUI : public QWidget {
    Q_OBJECT

public:
    explicit EditorUI(QWidget* parent = nullptr);
    void applyTheme(const QString& themeName);

private slots:
    void onAIResponseReceived(const QString& response);
    void onFormatCode();
    void onTextChanged();
    void requestCompletion();
    void insertCompletion(const QString& completion);

private:
    QPlainTextEdit* editor;
    AIAssistant* aiAssistant;
    SyntaxHighlighter* syntaxHighlighter;
    CodeFormatter* codeFormatter;
    QDockWidget* chatDock;
    QCompleter* completer;
    QTimer* debounceTimer;

    void setupUI();
    void setupConnections();
    void setupShortcuts();
    void showCompletionPopup(const QStringList& suggestions);
};

#endif // EDITOR_UI_H
