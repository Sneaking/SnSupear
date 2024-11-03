// config_manager.cpp
#include "config_manager.h"

#include <QFile>
#include <QJsonDocument>
#include <QSettings>
#include <QDir>
#include <QDebug>

/**
 * @brief Private constructor to enforce singleton pattern.
 */
ConfigManager::ConfigManager() {}

/**
 * @brief Gets the singleton instance of the ConfigManager.
 * @return Reference to the ConfigManager instance.
 */
ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

/**
 * @brief Gets the OpenAI API key.
 * @return The API key as a QString.
 */
QString ConfigManager::getAPIKey() const {
    QSettings settings;
    return settings.value("openai/apiKey").toString();
}

/**
 * @brief Gets a setting value by key.
 * @param key The setting key.
 * @param defaultValue The default value to return if the key is not found.
 * @return The setting value or the default value.
 */
QVariant ConfigManager::getSetting(const QString& key, const QVariant& defaultValue) const {
    QSettings settings;
    return settings.value(key, defaultValue);
}

/**
 * @brief Sets a setting value by key.
 * @param key The setting key.
 * @param value The value to set.
 */
void ConfigManager::setSetting(const QString& key, const QVariant& value) {
    QSettings settings;
    settings.setValue(key, value);
}

/**
 * @brief Gets the syntax highlighting rules for the given language.
 * @param language The language identifier.
 * @param forceRefresh If true, forces reloading of the rules from the configuration file.
 * @return A QJsonObject containing the syntax highlighting rules.
 */
QJsonObject ConfigManager::getSyntaxRules(const QString& language, bool forceRefresh) {
    Q_UNUSED(forceRefresh); // Placeholder for potential future implementation

    if (language == "cpp") {
        QJsonObject syntaxRules;

        // Keywords
        syntaxRules["keyword"] = QJsonObject{
            {"pattern", "\\b(auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)\\b"},
            {"color", "#007bff"},
            {"bold", true}
        };

        // Types
        syntaxRules["type"] = QJsonObject{
            {"pattern", "\\b(bool|int|char|float|double|void)\\b"},
            {"color", "#673ab7"},
            {"bold", false}
        };

        // Strings
        syntaxRules["string"] = QJsonObject{
            {"pattern", "\"(?:[^\\\"]|\\.)*\""},
            {"color", "#e91e63"},
            {"bold", false}
        };

        // Comments
        syntaxRules["comment"] = QJsonObject{
            {"pattern", "//[^\n]*"},
            {"color", "#9e9e9e"},
            {"bold", false}
        };

        return syntaxRules;
    } else {
        // Handle other languages as needed
        return QJsonObject();
    }
}

/**
 * @brief Gets the theme configuration for the given theme name.
 * @param themeName The name of the theme.
 * @return A QJsonObject containing the theme configuration.
 */
QJsonObject ConfigManager::getTheme(const QString& themeName) {
    if (themeName == "Sn_MarinaSync_Dark") {
        return QJsonObject{
            {"background", "#000000"},
            {"foreground", "#00ff00"},
            {"keyword", "#00ffff"},
            {"operator", "#ff00ff"},
            {"string", "#ffff00"},
            {"comment", "#808080"},
            {"number", "#ff8000"},
            {"def", "#00ff00"},
            {"variable", "#00ff00"},
            {"variable2", "#00ffaa"},
            {"property", "#00ffff"}
            // ... add other color mappings as needed ...
        };
    } else if (themeName == "Sn_MarinaSync_Contrast") {
        return QJsonObject{
            {"background", "#000000"},
            {"foreground", "#ffff00"},
            {"keyword", "#00ffff"},
            {"operator", "#ff00ff"},
            {"string", "#00ff00"},
            {"comment", "#ffffff"},
            {"number", "#ffa500"},
            {"def", "#0080ff"},
            {"variable", "#8000ff"},
            {"variable2", "#00ff80"},
            {"property", "#ff0080"}
            // ... add other color mappings as needed ...
        };
    } else if (themeName == "Sn_MarinaSync_Light") {
        return QJsonObject{
            {"background", "#ffffff"},
            {"foreground", "#000000"},
            {"keyword", "#0000ff"},
            {"operator", "#800080"},
            {"string", "#800000"},
            {"comment", "#a0a0a0"},
            {"number", "#ff8000"},
            {"def", "#008000"},
            {"variable", "#000000"},
            {"variable2", "#008080"},
            {"property", "#0000ff"}
            // ... add other color mappings as needed ...
        };
    } else if (themeName == "Sn_MarinaSync_WCAG") {
        return QJsonObject{
            {"background", "#ffffff"},
            {"foreground", "#000000"},
            {"keyword", "#0078d7"},
            {"operator", "#c71585"},
            {"string", "#107c10"},
            {"comment", "#696969"},
            {"number", "#a80000"},
            {"def", "#264f78"},
            {"variable", "#000000"},
            {"variable2", "#5c2d91"},
            {"property", "#0078d7"}
            // ... add other color mappings as needed ...
        };
    } else {
        // Handle other themes as needed
        return QJsonObject{};
    }
}

/**
 * @brief Gets the syntax color mapping for the given language.
 * @param language The language identifier.
 * @return A QJsonObject mapping syntax elements to color names.
 */
QJsonObject ConfigManager::getSyntaxColors(const QString& language) {
    if (language == "cpp") {
        return QJsonObject{
            {"keyword", "blue1"},
            {"string", "green2"},
            {"comment", "blue2"},
            {"operator", "orange"},
            {"number", "orange"},
            {"preprocessor", "pink"},
            {"type", "red"},
            {"function", "blueVibrant"}
        };
    } else {
        // Handle other languages as needed
        return QJsonObject{};
    }
}
