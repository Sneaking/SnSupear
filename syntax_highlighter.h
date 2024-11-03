// syntax_highlighter.h
#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QJsonObject>

/**
 * @brief Syntax highlighter class for the code editor.
 *
 * Provides language-specific syntax highlighting using regular expressions
 * and configurable rules loaded from external configuration files.
 */
class SyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    /**
     * @brief Constructs a syntax highlighter for the given text document.
     * @param parent The text document to highlight.
     */
    explicit SyntaxHighlighter(QTextDocument* parent = nullptr);

    /**
     * @brief Sets the programming language for syntax highlighting.
     * @param language The language identifier (e.g., "cpp", "python").
     */
    void setLanguage(const QString& language);

    /**
     * @brief Sets the highlighting rules based on the given theme.
     * @param theme The theme configuration as a QJsonObject.
     */
    void setHighlightingRules(const QJsonObject& theme);

protected:
    /**
     * @brief Highlights a single block of text.
     * @param text The text block to highlight.
     */
    void highlightBlock(const QString& text) override;

private:
    /**
     * @brief Loads language-specific highlighting rules from configuration.
     * @param language The language identifier.
     */
    void loadLanguageRules(const QString& language);

    /**
     * @brief Creates a QTextCharFormat with the specified color and bold setting.
     * @param color The color to use for the format.
     * @param bold Whether the text should be bold.
     * @return The created QTextCharFormat.
     */
    QTextCharFormat createTextFormat(const QString& color, bool bold = false, bool italic = false);

    struct HighlightingRule {
        QRegularExpression pattern;  ///< Regular expression pattern to match.
        QTextCharFormat format;      ///< Text format to apply when the pattern matches.
    };

    QVector<HighlightingRule> highlightingRules;  ///< Collection of active highlighting rules.
    QString currentLanguage;                       ///< Currently active language.
};
