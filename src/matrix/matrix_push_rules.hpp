#pragma once

#include <QObject>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QHash>

namespace pcp_chat {

enum class PushAction {
    Notify,
    DontNotify,
    Coalesce,
    SetTweak,
    Unknown
};

struct PushCondition {
    QString kind;           // "event_match", "contains_display_name", "room_member_count"
    QString key;            // e.g., "content.body"
    QString pattern;        // Glob pattern
    QString is;             // Exact match
    QJsonObject raw;
};

struct PushRule {
    QString ruleId;
    bool isDefault = true;
    bool enabled = true;
    int priority = 0;
    QString kind;           // "override", "underride", "content", "room", "sender"
    QString rule;           // The actual rule pattern (room_id, sender, etc.)
    QList<PushAction> actions;
    QList<PushCondition> conditions;
    QHash<QString, QString> tweaks; // sound, highlight, color
    QJsonObject raw;
};

class MatrixPushRules : public QObject
{
    Q_OBJECT

public:
    explicit MatrixPushRules(QObject *parent = nullptr);
    ~MatrixPushRules() override;

    void loadGlobalRules(const QJsonObject &data);
    void mergeFromSync(const QJsonObject &data);

    QList<PushRule> matchingRules(const QString &roomId, const QString &sender,
                                   bool isDirect, bool isHighlighted,
                                   const QString &body, int memberCount) const;

    QList<PushRule> allRules() const { return m_globalRules; }
    bool isNoisyRule(const PushRule &rule) const;
    bool shouldNotify(const PushRule &rule) const;
    QString soundForRule(const PushRule &rule) const;
    QString highlightColor(const PushRule &rule) const;

    static PushRule parseRule(const QJsonObject &obj);
    static QJsonObject serializeRule(const PushRule &rule);

private:
    QList<PushRule> m_globalRules;
    QHash<QString, PushRule> m_roomRules; // Per-room overrides
};

} // namespace pcp_chat
