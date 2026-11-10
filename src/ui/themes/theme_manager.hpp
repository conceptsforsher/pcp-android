#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QHash>
#include <QMap>
#include <QColor>

namespace pcp_chat {

class ThemeManager : public QObject
{
    Q_OBJECT

public:
    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager() override;

    enum class Theme { Light, Dark, HighContrast, Retro, Custom };

    void loadTheme(const QString &path);
    void saveTheme(const QString &path);
    void applyTheme(Theme theme);
    void applySystemTheme();
    void setCustomColors(const QMap<QString, QColor> &colors);

    QString activeThemeName() const;
    Theme activeTheme() const { return m_activeTheme; }

    // Color accessors
    QColor backgroundColor() const;
    QColor surfaceColor() const;
    QColor primaryColor() const;
    QColor textColor() const;
    QColor secondaryTextColor() const;
    QColor borderColor() const;
    QColor errorColor() const;
    QColor warningColor() const;
    QColor successColor() const;
    QColor linkColor() const;
    QColor highlightColor() const;
    QColor sentMessageColor() const;
    QColor receivedMessageColor() const;

    QString stylesheet() const { return m_stylesheet; }
    void setStylesheet(const QString &ss);

signals:
    void themeChanged(Theme theme);
    void colorsUpdated();

private:
    void initDefaultThemes();
    void generateStylesheet();
    QString colorToString(QColor color) const;

    Theme m_activeTheme = Theme::Light;
    QString m_stylesheet;
    QMap<QString, QColor> m_colors;
};

} // namespace pcp_chat
