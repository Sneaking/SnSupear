// EditorUI.cpp
#include "EditorUI.h"
#include <QPalette>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include "ConfigManager.h"
#include <QScrollBar>
#include <QCompleter>
#include <QShortcut>
#include <QTimer>
#include <QStringListModel>

EditorUI::EditorUI(QWidget* parent) : QWidget(parent),
    editor(new QPlainTextEdit(this)),
    aiAssistant(new AIAssistant(this)),
    syntaxHighlighter(new SyntaxHighlighter(editor->document())),
    codeFormatter(new CodeFormatter(this)),
    chatDock(new QDockWidget("AI Chat", this)),
    completer(new QCompleter(this)),
    debounceTimer(new QTimer(this))
{
    setupUI();
    setupConnections();
    setupShortcuts();

    // Apply a default theme on startup
    applyTheme("Sn_MarinaSync_Dark"); 

    // Initialize AI Assistant with API key from ConfigManager
    QString apiKey = ConfigManager::getInstance().getAPIKey();
    aiAssistant->setApiKey(apiKey);
}

void EditorUI::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(editor);

    // Set up the chat dock widget
    chatDock->setAllowedAreas(Qt::BottomDockWidgetArea); 
    chatDock->setWidget(aiAssistant);  
    addDockWidget(Qt::BottomDockWidgetArea, chatDock); 

    setLayout(layout);

    // Set up QCompleter
    completer->setWidget(editor);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
}

void EditorUI::setupConnections() {
    connect(aiAssistant, &AIAssistant::responseReceived, this, &EditorUI::onAIResponseReceived);
    connect(editor, &QPlainTextEdit::textChanged, this, [this]() {
        syntaxHighlighter->rehighlight();
        onTextChanged(); // Trigger code completion
    });
    connect(this, &EditorUI::customContextMenuRequested, this, &EditorUI::onFormatCode);
    connect(completer, QOverload<const QString &>::of(&QCompleter::activated),
            this, &EditorUI::insertCompletion);
}

void EditorUI::setupShortcuts() {
    // Add shortcuts for code completion (e.g., Ctrl+Space) and formatting (e.g., Ctrl+I)
    QShortcut *completionShortcut = new QShortcut(QKeySequence("Ctrl+Space"), this);
    connect(completionShortcut, &QShortcut::activated, this, &EditorUI::requestCompletion);

    QShortcut *formatShortcut = new QShortcut(QKeySequence("Ctrl+I"), this);
    connect(formatShortcut, &QShortcut::activated, this, &EditorUI::onFormatCode);
}

void EditorUI::applyTheme(const QString& themeName) {
    QJsonObject theme = ConfigManager::getInstance().getTheme(themeName);
    if (!theme.isEmpty()) {
        QPalette palette = editor->palette();
        palette.setColor(QPalette::Base, QColor(theme["background"].toString()));
        palette.setColor(QPalette::Text, QColor(theme["foreground"].toString()));
        editor->setPalette(palette);

        syntaxHighlighter->setHighlightingRules(theme); 
    } else {
        qWarning() << "Theme not found:" << themeName;
    }
}

void EditorUI::onAIResponseReceived(const QString& response) {
    editor->appendPlainText("\nAI Response:\n" + response);
    QScrollBar *verticalScrollBar = editor->verticalScrollBar();
    verticalScrollBar->setValue(verticalScrollBar->maximum());
}

void EditorUI::onFormatCode() {
    QString formattedCode = codeFormatter->formatCode(editor->toPlainText(), "cpp");
    editor->setPlainText(formattedCode);
}

void EditorUI::onTextChanged() {
    if (completer->popup()->isVisible()) {
        completer->popup()->hide();
    }

    debounceTimer->start(300); // Debounce timer for 300ms
}

void EditorUI::requestCompletion() {
    QString prompt = editor->toPlainText();
    QTextCursor tc = editor->textCursor();
    int cursorPosition = tc.position();

    aiAssistant->getSuggestions(prompt.toStdString(), cursorPosition, [this](const std::string& suggestions) {
        QStringList suggestionList = QString::fromStdString(suggestions).split('\n', Qt::SkipEmptyParts);
        showCompletionPopup(suggestionList);
    });
}

void EditorUI::showCompletionPopup(const QStringList& suggestions) {
    if (suggestions.isEmpty())
        return;

    completer->setModel(new QStringListModel(suggestions, completer));
    completer->setCompletionPrefix(editor->textUnderCursor());

    QRect cr = editor->cursorRect();
    cr.setWidth(completer->popup()->sizeHintForColumn(0)
                + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cr);
}

void EditorUI::insertCompletion(const QString& completion) {
    QTextCursor tc = editor->textCursor();
    tc.insertText(completion);
    editor->setTextCursor(tc);
}
