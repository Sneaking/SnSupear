// code_formatter.h
#pragma once

#include <QString>
#include <functional>

/**
 * @brief Code formatter class for formatting code using external tools.
 */
class CodeFormatter {
public:
    /**
     * @brief Callback function type for handling the formatted code.
     * @param formattedCode The formatted code.
     * @param success True if formatting was successful, false otherwise.
     */
    using FormatCallback = std::function<void(const QString&, bool)>;

    /**
     * @brief Formats the given code using an external formatter.
     * @param code The code to format.
     * @param callback Callback function to be called with the formatted code.
     */
    void format(const QString& code, FormatCallback callback);

    /**
     * @brief Formats the given code using clang-format.
     * @param code The code to be formatted.
     * @return The formatted code as a QString.
     */
    QString formatCode(const QString& code);
};
