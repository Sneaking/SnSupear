// code_formatter.cpp
#include "code_formatter.h"

#include <QProcess>
#include <QTemporaryFile>
#include <QDebug>

/**
 * @brief Formats the given code using an external formatter.
 * @param code The code to format.
 * @param callback Callback function to be called with the formatted code.
 */
void CodeFormatter::format(const QString& code, FormatCallback callback) {
    // Create a temporary file to store the code
    QTemporaryFile tempFile;
    if (!tempFile.open()) {
        qWarning() << "Failed to create temporary file for formatting.";
        callback("", false);  // Call the callback with an empty string and failure status
        return;
    }

    // Write the code to the temporary file
    if (tempFile.write(code.toUtf8()) == -1) {
        qWarning() << "Failed to write to temporary file.";
        callback("", false);
        return;
    }
    tempFile.close();  // Close the file after writing

    // Start the external formatter process (e.g., clang-format)
    QProcess formatter;
    formatter.start("clang-format", QStringList() << "-i" << tempFile.fileName());
    formatter.waitForFinished();  // Wait for the formatter to finish

    // Check if the formatter was successful
    if (formatter.exitCode() != 0) {
        qWarning() << "Code formatting failed.";
        callback("", false);
        return;
    }

    // Reopen the temporary file to read the formatted code
    if (!tempFile.open()) {
        qWarning() << "Failed to open temporary file after formatting.";
        callback("", false);
        return;
    }

    // Read the formatted code from the temporary file
    QString formattedCode = QString::fromUtf8(tempFile.readAll());
    callback(formattedCode, true);  // Call the callback with the formatted code and success status
}

/**
 * @brief Formats the given code using clang-format.
 * @param code The code to be formatted.
 * @return The formatted code as a QString.
 */
QString CodeFormatter::formatCode(const QString& code) {
    QString formattedCode;
    format(code, [&formattedCode](const QString& result, bool success) {
        if (success) {
            formattedCode = result;
        } else {
            qWarning() << "Error formatting code.";
        }
    });
    return formattedCode;
}
