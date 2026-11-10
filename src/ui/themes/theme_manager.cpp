#include "theme_manager.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include <QDebug>

namespace pcp_chat {

ThemeManager::ThemeManager(QObject *parent) : QObject(parent)
{
    initDefaultThemes();
    applyTheme(Theme::Light);
}

ThemeManager::~ThemeManager() = default;

void ThemeManager::initDefaultThemes()
{
    // Light theme defaults
    // Colors loaded from built-in palette
}

void ThemeManager::loadTheme(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    QJsonObject colors = obj["colors"].toObject();

    for (auto it = colors.begin(); it != colors.end(); ++it) {
        m_colors[it.key()] = QColor(it.value().toString());
    }

    if (obj.contains("stylesheet"))
        m_stylesheet = obj["stylesheet"].toString();

    generateStylesheet();
    emit colorsUpdated();
}

void ThemeManager::saveTheme(const QString &path)
{
    QJsonObject obj;
    QJsonObject colorsObj;

    for (auto it = m_colors.begin(); it != m_colors.end(); ++it) {
        colorsObj[it.key()] = it.value().name();
    }
    obj["colors"] = colorsObj;
    obj["stylesheet"] = m_stylesheet;

    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    }
}

void ThemeManager::applyTheme(Theme theme)
{
    m_activeTheme = theme;

    switch (theme) {
        case Theme::Dark:
            m_colors["background"] = QColor("#1e1e1e");
            m_colors["surface"] = QColor("#2d2d2d");
            m_colors["primary"] = QColor("#4CAF50");
            m_colors["text"] = QColor("#e0e0e0");
            m_colors["text_secondary"] = QColor("#a0a0a0");
            m_colors["border"] = QColor("#404040");
            m_colors["error"] = QColor("#f44336");
            m_colors["warning"] = QColor("#ff9800");
            m_colors["success"] = QColor("#4caf50");
            m_colors["link"] = QColor("#64b5f6");
            m_colors["highlight"] = QColor("#66bb6a");
            m_colors["sent_message"] = QColor("#1b5e20");
            m_colors["received_message"] = QColor("#263238");
            break;

        case Theme::HighContrast:
            m_colors["background"] = QColor("#000000");
            m_colors["surface"] = QColor("#1a1a1a");
            m_colors["primary"] = QColor("#00ff00");
            m_colors["text"] = QColor("#ffffff");
            m_colors["text_secondary"] = QColor("#cccccc");
            m_colors["border"] = QColor("#ffffff");
            m_colors["sent_message"] = QColor("#003300");
            m_colors["received_message"] = QColor("#000033");
            break;

        case Theme::Retro:
            m_colors["background"] = QColor("#000080");
            m_colors["surface"] = QColor("#c0c0c0");
            m_colors["primary"] = QColor("#ffff00");
            m_colors["text"] = QColor("#ffffff");
            m_colors["text_secondary"] = QColor("#c0c0c0");
            m_colors["border"] = QColor("#808080");
            m_colors["sent_message"] = QColor("#008000");
            m_colors["received_message"] = QColor("#800000");
            break;

        case Theme::Light:
        default:
            m_colors["background"] = QColor("#ffffff");
            m_colors["surface"] = QColor("#f5f5f5");
            m_colors["primary"] = QColor("#4CAF50");
            m_colors["text"] = QColor("#212121");
            m_colors["text_secondary"] = QColor("#757575");
            m_colors["border"] = QColor("#e0e0e0");
            m_colors["error"] = QColor("#d32f2f");
            m_colors["warning"] = QColor("#f57c00");
            m_colors["success"] = QColor("#388e3c");
            m_colors["link"] = QColor("#1976d2");
            m_colors["highlight"] = QColor("#81c784");
            m_colors["sent_message"] = QColor("#e8f5e9");
            m_colors["received_message"] = QColor("#e3f2fd");
            break;
    }

    generateStylesheet();
    emit themeChanged(m_activeTheme);
    emit colorsUpdated();
}

void ThemeManager::applySystemTheme()
{
    // Detect system dark/light preference
    QPalette appPalette = QApplication::palette();
    bool isDark = appPalette.color(QPalette::Window).lightness() < 128;

    applyTheme(isDark ? Theme::Dark : Theme::Light);
}

void ThemeManager::setCustomColors(const QMap<QString, QColor> &colors)
{
    m_colors = colors;
    m_activeTheme = Theme::Custom;
    generateStylesheet();
    emit themeChanged(m_activeTheme);
    emit colorsUpdated();
}

QString ThemeManager::activeThemeName() const
{
    switch (m_activeTheme) {
        case Theme::Light: return "Light";
        case Theme::Dark: return "Dark";
        case Theme::HighContrast: return "High Contrast";
        case Theme::Retro: return "Retro";
        case Theme::Custom: return "Custom";
    }
    return "Unknown";
}

void ThemeManager::generateStylesheet()
{
    m_stylesheet = QString(
        "* { color: %1; background-color: %2; }"
        "QMainWindow { background-color: %2; }"
        "QMenuBar { background-color: %3; color: %1; }"
        "QMenu { background-color: %3; color: %1; }"
        "QMenu::item:selected { background-color: %4; }"
        "QPushButton { background-color: %4; color: white; border: none; "
        "border-radius: 4px; padding: 8px 16px; }"
        "QPushButton:hover { background-color: rgba(76, 175, 80, 0.8); }"
        "QLineEdit { border: 1px solid %5; border-radius: 4px; padding: 6px; }"
        "QListWidget { border: 1px solid %5; background-color: %2; }"
        "QTextBrowser { border: none; background-color: %2; }"
    ).arg(
        colorToString(textColor()),
        colorToString(backgroundColor()),
        colorToString(surfaceColor()),
        colorToString(primaryColor()),
        colorToString(borderColor())
    );
}

QString ThemeManager::colorToString(QColor color) const
{
    return color.name();
}

QColor ThemeManager::backgroundColor() const { return m_colors.value("background", QColor("#ffffff")); }
QColor ThemeManager::surfaceColor() const { return m_colors.value("surface", QColor("#f5f5f5")); }
QColor ThemeManager::primaryColor() const { return m_colors.value("primary", QColor("#4CAF50")); }
QColor ThemeManager::textColor() const { return m_colors.value("text", QColor("#212121")); }
QColor ThemeManager::secondaryTextColor() const { return m_colors.value("text_secondary", QColor("#757575")); }
QColor ThemeManager::borderColor() const { return m_colors.value("border", QColor("#e0e0e0")); }
QColor ThemeManager::errorColor() const { return m_colors.value("error", QColor("#d32f2f")); }
QColor ThemeManager::warningColor() const { return m_colors.value("warning", QColor("#f57c00")); }
QColor ThemeManager::successColor() const { return m_colors.value("success", QColor("#388e3c")); }
QColor ThemeManager::linkColor() const { return m_colors.value("link", QColor("#1976d2")); }
QColor ThemeManager::highlightColor() const { return m_colors.value("highlight", QColor("#81c784")); }
QColor ThemeManager::sentMessageColor() const { return m_colors.value("sent_message", QColor("#e8f5e9")); }
QColor ThemeManager::receivedMessageColor() const { return m_colors.value("received_message", QColor("#e3f2fd")); }

void ThemeManager::setStylesheet(const QString &ss)
{
    m_stylesheet = ss;
}

} // namespace pcp_chat
