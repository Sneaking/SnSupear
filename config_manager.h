#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

/**
 * @brief Manages application configuration and settings.
 */
class ConfigManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Gets the singleton instance of the ConfigManager.
     * @return Reference to the ConfigManager instance.
     */
    static ConfigManager& getInstance();

    /**
     * @brief Gets the OpenAI API key.
     * @return The API key as a QString.
     */
    QString getAPIKey() const;

    /**
     * @brief Gets a setting value by key.
     * @param key The setting key.
     * @param defaultValue The default value to return if the key is not found.
     * @return The setting value or the default value.
     */
    QVariant getSetting(const QString& key, const QVariant& defaultValue = QVariant()) const;

    /**
     * @brief Sets a setting value by key.
     * @param key The setting key.
     * @param value The value to set.
     */
    void setSetting(const QString& key, const QVariant& value);

    /**
     * @brief Gets the syntax highlighting rules for the given language.
     * @param language The language identifier.
     * @param forceRefresh If true, forces reloading of the rules from the configuration file.
     * @return A QJsonObject containing the syntax highlighting rules.
     */
    QJsonObject getSyntaxRules(const QString& language, bool forceRefresh = false);

    /**
     * @brief Gets the theme configuration for the given theme name.
     * @param themeName The name of the theme.
     * @return A QJsonObject containing the theme configuration.
     */
    QJsonObject getTheme(const QString& themeName);

    /**
     * @brief Gets the syntax color mapping for the given language.
     * @param language The language identifier.
     * @return A QJsonObject mapping syntax elements to color names.
     */
    QJsonObject getSyntaxColors(const QString& language);

private:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    ConfigManager();
};

#endif // CONFIG_MANAGER_H
