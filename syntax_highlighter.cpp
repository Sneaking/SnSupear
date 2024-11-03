// syntax_highlighter.cpp
#include "syntax_highlighter.h"
#include "config_manager.h"
#include <QDebug>

/**
 * @brief Constructs a syntax highlighter for the given text document.
 * @param parent The text document to highlight.
 */
SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{}

/**
 * @brief Sets the programming language for syntax highlighting.
 * @param language The language identifier (e.g., "cpp", "python").
 */
void SyntaxHighlighter::setLanguage(const QString &language) {
    if (currentLanguage != language) {
        currentLanguage = language;
        loadLanguageRules(language);
        rehighlight();
    }
}

/**
 * @brief Sets the highlighting rules based on the given theme.
 * @param theme The theme configuration as a QJsonObject.
 */
void SyntaxHighlighter::setHighlightingRules(const QJsonObject& theme) {
    highlightingRules.clear();

    if (currentLanguage.isEmpty()) {
        return;
    }

    QJsonObject syntaxColors = ConfigManager::getInstance().getSyntaxColors(currentLanguage);
    if (syntaxColors.isEmpty()) {
        qWarning() << "No syntax colors found for language:" << currentLanguage;
        return;
    }

    for (const QString& key : syntaxColors.keys()) {
        QString colorName = syntaxColors[key].toString();
        if (theme.contains(colorName)) {
            HighlightingRule rule;
            rule.pattern = QRegularExpression(key); // Assuming keys are the patterns
            rule.format = createTextFormat(theme[colorName].toString());
            highlightingRules.append(rule);
        } else {
            qWarning() << "Color not found in theme:" << colorName;
        }
    }
}

/**
 * @brief Highlights a single block of text.
 * @param text The text block to highlight.
 */
void SyntaxHighlighter::highlightBlock(const QString &text) {
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

/**
 * @brief Loads language-specific highlighting rules from configuration.
 * @param language The language identifier.
 */
void SyntaxHighlighter::loadLanguageRules(const QString &language) {
    highlightingRules.clear();

    QJsonObject syntaxRules = ConfigManager::getInstance().getSyntaxRules(language);
    if (syntaxRules.isEmpty()) {
        return;
    }

    for (const QString& key : syntaxRules.keys()) {
        QJsonObject rule = syntaxRules[key].toObject();
        HighlightingRule highlightingRule;
        highlightingRule.pattern = QRegularExpression(rule["pattern"].toString());
        highlightingRule.format = createTextFormat(rule["color"].toString(), rule["bold"].toBool(false), rule["italic"].toBool(false));
        highlightingRules.append(highlightingRule);
    }
}

/**
 * @brief Creates a QTextCharFormat with the specified color, bold and italic settings.
 * @param color The color to use for the format.
 * @param bold Whether the text should be bold.
 * @param italic Whether the text should be italic.
 * @return The created QTextCharFormat.
 */
QTextCharFormat SyntaxHighlighter::createTextFormat(const QString &color, bool bold, bool italic) {
    QTextCharFormat format;
    format.setForeground(QColor(color));
    if (bold) {
        format.setFontWeight(QFont::Bold);
    }
    if (italic) {
        format.setFontItalic(true);
    }
    return format;
}
